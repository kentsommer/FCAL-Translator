/* A recursive descent parser with operator precedence.

   Author: Eric Van Wyk
   Modified: Robert Edge        

   This algorithm is based on the work of Douglas Crockford in "Top
   Down Operator Precedence", a chapter in the book "Beautiful Code".
   Mr. Crockford describes in his chapter how his work is based on an
   algorithm described Vaughan Pratt in "Top Down Operator
   Precedence", presented at the ACM Symposium on Principles of
   Programming Languages.

   Douglas Crockford's chapter is available at 
    http://javascript.crockford.com/tdop/tdop.html

   Vaughan Pratt's paper is available at 
    http://portal.acm.org/citation.cfm?id=512931

   These are both quite interesting works and worth reading if you
   find the problem of parsing to be an interesting one.

   Last modified: Oct 13, 2013.

*/

#include "parser.h"
#include "scanner.h"
#include "extToken.h"
#include <stdio.h>
#include <assert.h>
#include "AST.h"
#include <stdlib.h>
#include <string>
#include <cstring>

using namespace std;

/*!
 * Deconstructor for ther Parser.
 */
Parser::~Parser() 
{
    if (s) delete s;

    ExtToken *extTokenToDelete;
    currToken = tokens;
    while (currToken) 
    {
        extTokenToDelete = currToken;
        currToken = currToken->next;
        delete extTokenToDelete;
    }

    Token *tokenToDelete;
    Token *currScannerToken = stokens;
    while (currScannerToken) 
    {
        tokenToDelete = currScannerToken;
        currScannerToken = currScannerToken->next;
        delete tokenToDelete;
    }

}

/*!
 * The constructor of Parser, which takes in text and and contstruct a new parser.
 */
Parser::Parser() 
{ 
    currToken = NULL; prevToken = NULL; tokens = NULL; 
    s = NULL; stokens = NULL; 
}

ParseResult Parser::parse (const char *text) 
{
    assert (text != NULL);

    ParseResult pr;
    try 
    {
        s = new Scanner();
        stokens = s->scan (text);        
        tokens = extendTokenList (this, stokens);

        assert (tokens != NULL);
        currToken = tokens;
        pr = parseProgram();
    }
    catch (string errMsg) 
    {
        pr.ok = false;
        pr.errors = errMsg;
        pr.ast = NULL;
    }
    return pr;
}

/*!
 * Match the entire program and return a parse result.
 */
ParseResult Parser::parseProgram() 
{
    ParseResult pr;
    // Program ::= nameKwd colon variableName semiColon Platform Decls States
    match(variableName);
    string str(prevToken->lexeme);
    match(leftParen);
    match(rightParen);
    match(leftCurly);
    ParseResult prStmts = parseStmts();
    match(rightCurly);
    match(endOfFile);
	Stmts* sstmts1 = NULL;
	if(prStmts.ast)
	{
		sstmts1 = dynamic_cast<Stmts *>(prStmts.ast);
		if(!sstmts1)throw((string) "Failed cast: stmts in parseProgram");
	}
    pr.ast = new Tprogram(str, sstmts1);
	return pr;
}

/*!
 * This method matches:
 * Decl ::= 'Matrix' varName '[' Expr ',' Expr ']' varName ',' varName  '=' Expr ';'
 * or
 * Decl ::= 'Matrix' varName '=' Expr ';'
 */
ParseResult Parser::parseMatrixDecl() 
{
    ParseResult pr;
    match(matrixKwd);
    match(variableName);
    string str1 = prevToken->lexeme;
    // Decl ::= 'Matrix' varName '[' Expr ',' Expr ']' varName ',' varName  '=' Expr ';'
	if(attemptMatch(leftSquare))
	{
		ParseResult expr1 = parseExpr(0);
		match(comma);
		ParseResult expr2 = parseExpr(0);
		match(rightSquare);
		parseVariableName();
		string str2 = prevToken->lexeme;
		match(comma);
		parseVariableName();
		string str3 = prevToken->lexeme;
		match(assign);
		ParseResult expr3 = parseExpr(0);

		Expr *ex1 = NULL, *ex2 = NULL, *ex3 = NULL;
		if(expr1.ast && expr2.ast && expr3.ast)
		{
			ex1 = dynamic_cast<Expr *>(expr1.ast);
			if(!ex1)throw((string) "Failed cast: expr in parseMatrixDecl");

			ex2 = dynamic_cast<Expr *>(expr2.ast);
			if(!ex2)throw((string) "Failed cast: expr in parseMatrixDecl");

			ex3 = dynamic_cast<Expr *>(expr3.ast);
			if(!ex3)throw((string) "Failed cast: expr in parseMatrixDecl");
		}
		pr.ast = new MatrixDecl(str1, ex1, ex2, str2, str3, ex3);
    }
	// Decl ::= 'Matrix' varName '=' Expr ';'
	else if(attemptMatch(assign))
	{
		ParseResult expr4 = parseExpr(0);

		Expr* ex4 = NULL;
		if(expr4.ast)
		{
			ex4 = dynamic_cast<Expr *>(expr4.ast);
			if(!ex4)throw((string) "Failed cast: expr in parseMatrixDecl");
		}
		pr.ast = new AssignMatrixDecl(str1, ex4);
    }
	else
	{
    	throw ((string) "Failed. Bad matrix syntax");
	}	

    match(semiColon);

    return pr;
}

/*!
 * This method parses standard declarations:
 * Type ::= intKwd
 * Type ::= floatKwd
 * Type ::= stringKwd
 * Type ::= boolKwd
 */
ParseResult Parser::parseStandardDecl()
{
    ParseResult pr;
    string kwd;
	//ParseResult prType = parseType();

    if (attemptMatch(intKwd)) 
    {
        // Type ::= intKwd
		kwd = prevToken->lexeme;
    } 
    else if (attemptMatch(floatKwd)) 
    {
        // Type ::= floatKwd
		kwd = prevToken->lexeme;
    }
    else if (attemptMatch(stringKwd)) 
    {
        // Type ::= stringKwd
		kwd = prevToken->lexeme;
    }
    else if (attemptMatch(boolKwd)) 
    {
        // Type ::= boolKwd
        kwd = prevToken->lexeme;
    }
    match(variableName);
    string str = prevToken->lexeme;
    match(semiColon);
    pr.ast = new StandardDecl(kwd, str);
    return pr;
}

/*!
 * This method parses either matrix declaration:
 * Decl :: Matrix variableName ....
 * or:
 * Decl ::= Type variableName semiColon
 */
ParseResult Parser::parseDecl() 
{ 
    ParseResult pr;
	// Decl :: Matrix variableName ....
	if(nextIs(matrixKwd))
	{
		pr = parseMatrixDecl();
	} 
	// Decl ::= Type variableName semiColon
	else
	{
    	pr = parseStandardDecl();
	}
    return pr;
}

/*!
 * This method parses statements:
 * Decl ::= Type variableName semiColon
 * or null statements:
 * Stmts ::= 
 */
ParseResult Parser::parseStmts() 
{
    ParseResult pr;
    if (!nextIs(rightCurly) && !nextIs(inKwd)) 
    {
        // Stmts ::= Stmt Stmts
        ParseResult prStmt = parseStmt();
        ParseResult prStmts = parseStmts();
		pr.ast = new ConsStmts((Stmt*)prStmt.ast, (Stmts*)prStmts.ast);
    }
    else 
    {
        // Stmts ::= 
        // nothing to match.
		pr.ast = new NullStmts();
    }
    return pr;
}

/*!
 * This method parses the following statements:
 * Stmt ::= Decl
 * Stmt ::= '{' Stmts '}'
 * Stmt ::= 'if' '(' Expr ')' Stmt 
 * Stmt ::= 'if' '(' Expr ')' Stmt 'else' Stmt
 * Stmt ::= varName '=' Expr ';'  | varName '[' Expr ',' Expr ']' '=' Expr ';'	
 * Stmt ::= 'print' '(' Expr ')' ';'
 * Stmt ::= 'for' '(' varName '=' Expr ':' Expr ')' Stmt
 * Stmt ::= 'while' '(' Expr ')' Stmt
 * Stmt ::= ';'
 *
 */
ParseResult Parser::parseStmt() 
{
    ParseResult pr;

	//Stmt ::= Decl
	if(nextIs(intKwd)||nextIs(floatKwd)||nextIs(matrixKwd)||nextIs(stringKwd)||nextIs(boolKwd))
	{ 
		ParseResult decl1 = parseDecl();

		Decl* d1 = NULL;
		if(decl1.ast)
		{
			d1 = dynamic_cast<Decl*>(decl1.ast);
			if(!d1)throw((string) "Failed cast: decl in parseStmt");
		}
		pr.ast = new DeclStmt(d1);
	}
	//Stmt ::= '{' Stmts '}'
	else if (attemptMatch(leftCurly))
	{
		ParseResult stmts1 = parseStmts(); 
		match(rightCurly);

		Stmts* sstmts1 = NULL;
		if(stmts1.ast)
		{
			sstmts1 = dynamic_cast<Stmts*>(stmts1.ast);
			if(!sstmts1)throw((string) "Failed cast: stmts in parseStmt");
		}
		pr.ast = new StmtsStmt(sstmts1);
	}	
	//Stmt ::= 'if' '(' Expr ')' Stmt
	//Stmt ::= 'if' '(' Expr ')' Stmt 'else' Stmt
	else if (attemptMatch(ifKwd))
	{
		match(leftParen);
		ParseResult expr1 = parseExpr(0);
		match(rightParen);
		ParseResult stmt1 = parseStmt();
		Expr* ex1 = NULL;
		Stmt* s1 = NULL;
		if(attemptMatch(elseKwd))
		{
			ParseResult stmt2 = parseStmt();

			Stmt *s2 = NULL;
			if(expr1.ast && stmt1.ast && stmt2.ast)
			{
				ex1 = dynamic_cast<Expr*>(expr1.ast);
				if(!ex1)throw((string) "Failed cast: expr in parseStmt");
	
				s1 = dynamic_cast<Stmt*>(stmt1.ast);
				if(!s1)throw((string) "Failed cast: stmt in parseStmt");
				
				s2 = dynamic_cast<Stmt*>(stmt2.ast);
				if(!s2)throw((string) "Failed cast: stmt in parseStmt");
			}
			pr.ast = new IfElseStmt(ex1, s1, s2);
		}
		else
		{

			if(expr1.ast && stmt1.ast)
			{
				ex1 = dynamic_cast<Expr*>(expr1.ast);
				if(!ex1)throw((string) "Failed cast: expr in parseStmt");
	
				s1 = dynamic_cast<Stmt*>(stmt1.ast);
				if(!s1)throw((string) "Failed cast: stmt in parseStmt");
			}
			pr.ast = new IfStmt(ex1, s1);
		}
	}
	//Stmt ::= varName '=' Expr ';'  | varName '[' Expr ',' Expr ']' '=' Expr ';'
	else if  (attemptMatch(variableName))
	{
		string str1 = prevToken->lexeme;
		ParseResult expr4;
		if (attemptMatch (leftSquare)) 
		{
			ParseResult expr2 = parseExpr(0);
	  		match (comma);
		    ParseResult expr3 = parseExpr (0);
			match  (rightSquare);
			match(assign);
			expr4 = parseExpr(0);
			match(semiColon);

			Expr *ex2 = NULL, *ex3 =  NULL, *ex4 = NULL;
			if(expr2.ast && expr3.ast && expr4.ast)
			{
				ex2 = dynamic_cast<Expr*>(expr2.ast);
				if(!ex2)throw((string) "Failed cast: expr in parseStmt");

				ex3 = dynamic_cast<Expr*>(expr3.ast);
				if(!ex3)throw((string) "Failed cast: expr in parseStmt");

				ex4 = dynamic_cast<Expr*>(expr4.ast);
				if(!ex4)throw((string) "Failed cast: expr in parseStmt");
			}
			pr.ast = new AssignmStmt(str1, ex2, ex3, ex4);
		}
		else
		{
			match(assign);
			expr4 = parseExpr(0);
			match(semiColon);

			Expr* ex4 = NULL;
			if(expr4.ast)
			{
				ex4 = dynamic_cast<Expr*>(expr4.ast);
				if(!ex4)throw((string) "Failed cast: expr in parseStmt");
			}
			pr.ast = new AssignStmt(str1, ex4);
		}
	}
	//Stmt ::= 'print' '(' Expr ')' ';'
	else if (attemptMatch(printKwd)) 
	{
		match (leftParen);
		ParseResult expr5 = parseExpr(0);
		match (rightParen);
    	match (semiColon);

		Expr* ex5 = NULL;
		if(expr5.ast)
		{
			ex5 = dynamic_cast<Expr*>(expr5.ast);
			if(!ex5)throw((string) "Failed cast: expr in parseStmt");
		}
		pr.ast = new PrintStmt(ex5);
	}
	//Stmt ::= 'for' '(' varName '=' Expr 'to' Expr ')' Stmt
	else if (attemptMatch(forKwd)) 
	{
		match (leftParen);
		match (variableName);
		string str2 = prevToken->lexeme;
		match (assign);
		ParseResult expr6 = parseExpr (0);
		match (colon);
		ParseResult expr7 = parseExpr (0);
		match (rightParen);
		ParseResult stmt3 = parseStmt();

		Expr *ex6 = NULL, *ex7 = NULL;
		Stmt* s3 = NULL;

		if(expr6.ast && expr7.ast && stmt3.ast)
		{
			ex6 = dynamic_cast<Expr*>(expr6.ast);
			if(!ex6)throw((string) "Failed cast: expr in parseStmt");
	
			ex7 = dynamic_cast<Expr*>(expr7.ast);
			if(!ex7)throw((string) "Failed cast: expr in parseStmt");

			s3 = dynamic_cast<Stmt*>(stmt3.ast);
			if(!s3)throw((string) "Failed cast: stmt in parseStmt");
		}
		pr.ast = new ForStmt(str2, ex6, ex7, s3);
	}

	else if (attemptMatch(whileKwd)) 
    {
        match(leftParen);
        ParseResult expr8 = parseExpr(0);
        match(rightParen);
        ParseResult stmt4 = parseStmt();

        Expr *ex8 = NULL;
        Stmt *sstmt4 = NULL;

        if(expr8.ast && stmt4.ast)
        {
            ex8 = dynamic_cast<Expr*>(expr8.ast);
            if(!ex8)throw((string) "Failed cast: expr in parseStmt");

            sstmt4 = dynamic_cast<Stmt*>(stmt4.ast);
            if(!sstmt4)throw((string) "Failed cast: sttmnt in parseStmt");
        }
        pr.ast = new WhileStmt(ex8, sstmt4);
    }

	//Stmt ::= ';
    else if (attemptMatch(semiColon)) 
    {
		pr.ast = new ColonStmt();
	}
	else
	{
		throw (makeErrorMsg (currToken->terminal) + " while parsing a statement");
	}
	// Stmt ::= variableName assign Expr semiColon
    return pr;
}

/*!
 * This method examines current token, without consuming it, to call its
 * associated parse methods.  The ExtToken objects have 'nud' and
 * 'led' methods that are dispatchers that call the appropriate
 * parse methods.
 */
ParseResult Parser::parseExpr (int rbp) 
{
    ParseResult left = currToken->nud();
   
    while (rbp < currToken->lbp()) 
    {
		left = currToken->led(left);
    }

    return left;
}


/*
 * parse methods for Expr productions
 * ----------------------------------
 */

/*!
 * This method parses true keyword:
 * Expr ::= trueKwd
 */
 ParseResult Parser::parseTrueKwd() 
 {
     ParseResult pr;
     match (trueKwd);
     pr.ast = new Bool(prevToken->lexeme);
     return pr;
 }

/*!
 * This method parses false keyword:
 * Expr ::= falseKwd
 */
 ParseResult Parser::parseFalseKwd() 
 {
     ParseResult pr;
     match (falseKwd);
     pr.ast = new Bool(prevToken->lexeme);
     return pr;
 }

/*!
 * This method parses integer constant:
 * Expr ::= intConst
 */
ParseResult Parser::parseIntConst() 
{
    ParseResult pr;
    match (intConst);
    pr.ast = new Constant(prevToken->lexeme);
    return pr;
}

/*!
 * This method parses float constant:
 * Expr ::= floatConst
 */
ParseResult Parser::parseFloatConst() 
{
    ParseResult pr;
    match (floatConst);
    pr.ast = new Constant(prevToken->lexeme);
    return pr;
}

/*!
 * This method parses string constant:
 * Expr ::= stringConst
 */
ParseResult Parser::parseStringConst() 
{
    ParseResult pr;
    match (stringConst);
    pr.ast = new Constant(prevToken->lexeme);  
    return pr;
}

/*!
 * This method parses variable names:
 * Expr ::= variableName 
 */
ParseResult Parser::parseVariableName() 
{
    ParseResult pr;
    match (variableName);
    string str = prevToken->lexeme;
    if(attemptMatch(leftSquare))
    {
		ParseResult expr1 = parseExpr(0);
		match(comma);
		ParseResult expr2 = parseExpr(0);
		match(rightSquare);

		Expr *ex1 = NULL, *ex2 = NULL;
		if(expr1.ast && expr2.ast)
		{
			ex1 = dynamic_cast<Expr*>(expr1.ast);
			if(!ex1)throw((string) "Failed cast: expr in parseVariableName");
			
			ex2 = dynamic_cast<Expr*>(expr2.ast);
			if(!ex2)throw((string) "Failed cast: expr in parseVariableName");
		}
		pr.ast = new VarNameMatrix(str, ex1, ex2);
	}
    else if(attemptMatch(leftParen))
    {
		ParseResult expr3 = parseExpr(0);
		match(rightParen);

		Expr* ex3 = NULL;
		if(expr3.ast)
		{
			ex3 = dynamic_cast<Expr*>(expr3.ast);
			if(!ex3)throw((string) "Failed cast: expr in parseVariableName");
		}
		pr.ast = new VarNameNested(str, ex3);
	}
	//Expr := variableName
	else
	{
		pr.ast = new VarName(str);
	}
    return pr;
}

/*!
 * This method parses nested expression:
 * Expr ::= leftParen Expr rightParen
 */
ParseResult Parser::parseNestedExpr() 
{
    ParseResult pr;
    match (leftParen);
    ParseResult expr1 = parseExpr(0); 
    match(rightParen);

	Expr* ex1 = NULL;
	if(expr1.ast)
	{
		ex1 = dynamic_cast<Expr*>(expr1.ast);
		if(!ex1)throw((string) "Failed cast: expr in parseNestedExpr");
	}
    pr.ast = new ExprExpr(ex1);
    return pr;
}

/*!
 * This method parses if expression:
 * Expr ::= 'if' Expr 'then' Expr 'else' Expr  
 */
ParseResult Parser::parseIfExpr()
{	
	 ParseResult pr; 
	
	match(ifKwd);
	ParseResult ifexp = parseExpr(0);
	match(thenKwd);
	ParseResult thenexp = parseExpr(0);
	match(elseKwd);
	ParseResult elseexp = parseExpr(0);

	Expr *ife = NULL, *thene = NULL, *elsee = NULL;
	if(ifexp.ast && thenexp.ast && elseexp.ast)
	{
		ife = dynamic_cast<Expr*>(ifexp.ast);
		if(!ife)throw((string) "Failed cast: expr in parseIfExpr");
		
		thene = dynamic_cast<Expr*>(thenexp.ast);
		if(!thene)throw((string) "Failed cast: expr in parseIfExpr");

		elsee = dynamic_cast<Expr*>(elseexp.ast);
		if(!elsee)throw((string) "Failed cast: expr in parseIfExpr");
	}
	pr.ast = new IfExpr(ife, thene, elsee);

    return pr;
}

/*!
 * This method parses let expression:
 * Expr ::= 'let' Stmts 'in' Expr 'end' 
 */
ParseResult Parser::parseLetExpr()
{
   ParseResult pr;
   match(letKwd);
   ParseResult stmts1 = parseStmts();
   match(inKwd);
   ParseResult expr1 = parseExpr(0);
   match(endKwd);

   Expr* ex1 = NULL;
   Stmts* sstmts1 = NULL;
   if(stmts1.ast && expr1.ast)
   {
		ex1 = dynamic_cast<Expr*>(expr1.ast);
		if(!ex1)throw((string) "Failed cast: expr in parseLetExpr");

		sstmts1 = dynamic_cast<Stmts*>(stmts1.ast);
		if(!sstmts1)throw((string) "Failed cast: stmts in parseLetExpr");
   }
   pr.ast = new LetExpr(sstmts1, ex1);
   
   return pr;
}

/*!
 * This method parses not expression
 * Expr ::= '!' Expr 
 */
ParseResult Parser::parseNotExpr()
{
    ParseResult pr;
    match (notOp);
    ParseResult notExp = parseExpr(0); 
	Expr* ex1 = NULL;
	if(notExp.ast)
	{
		ex1 = dynamic_cast<Expr*>(notExp.ast);
		if(!ex1)throw((string) "Failed cast: expr in parseNotExpr");
	}
    pr.ast = new NotExpr(ex1);
    return pr;

}

/*!
 * This method parses addition:
 * Expr ::= Expr plusSign Expr
 */
ParseResult Parser::parseAddition (ParseResult prLeft) 
{
    // parser has already matched left expression 
    ParseResult pr;
    match (plusSign);
    pr = parseExpr(prevToken->lbp()); 

	Expr *left = NULL, *right = NULL;
	if(prLeft.ast && pr.ast)
	{
		left = dynamic_cast<Expr*>(prLeft.ast);
		if(!left)throw((string) "Failed cast: expr in parseAddition");

		right = dynamic_cast<Expr*>(pr.ast);
		if(!right)throw((string) "Failed cast: expr in parseAddition");
	}
    pr.ast = new Addition(left , right);
    return pr;
}

/*!
 * This method parses multiplication:
 * Expr ::= Expr star Expr
 */
ParseResult Parser::parseMultiplication (ParseResult prLeft) 
{
    // parser has already matched left expression 
    ParseResult pr;
    match (star);
    pr = parseExpr(prevToken->lbp());

 	Expr *left = NULL, *right = NULL;
	if(prLeft.ast && pr.ast)
	{
		left = dynamic_cast<Expr*>(prLeft.ast);
		if(!left)throw((string) "Failed cast: expr in parseMultiplication");

		right = dynamic_cast<Expr*>(pr.ast);
		if(!right)throw((string) "Failed cast: expr in parseMultiplication");
	}
    pr.ast = new Multiplication(left , right);
    return pr;
}

/*!
 * This method parses substraction:
 * Expr ::= Expr dash Expr
 */
ParseResult Parser::parseSubtraction (ParseResult prLeft) 
{
    // parser has already matched left expression 
    ParseResult pr;
    match (dash);
    pr = parseExpr(prevToken->lbp()); 

	Expr *left = NULL, *right = NULL;
	if(prLeft.ast && pr.ast)
	{
		left = dynamic_cast<Expr*>(prLeft.ast);
		if(!left)throw((string) "Failed cast: expr in parseSubtraction");

		right = dynamic_cast<Expr*>(pr.ast);
		if(!right)throw((string) "Failed cast: expr in parseSubtraction");
	}
    pr.ast = new Subtraction(left, right);
    return pr;
}

/*!
 * This method parses division:
 * Expr ::= Expr forwardSlash Expr
 */
ParseResult Parser::parseDivision (ParseResult prLeft) 
{
    // parser has already matched left expression 
    ParseResult pr;
    match (forwardSlash);
    pr = parseExpr(prevToken->lbp()); 

	Expr *left = NULL, *right = NULL;
	if(prLeft.ast && pr.ast)
	{
		left = dynamic_cast<Expr*>(prLeft.ast);
		if(!left)throw((string) "Failed cast: expr in parseDivision");

		right = dynamic_cast<Expr*>(pr.ast);
		if(!right)throw((string) "Failed cast: expr in parseDivision");
	}
    pr.ast = new Division(left , right);
    return pr;
}


// Expr ::= Expr equalEquals Expr
// Expr ::= Expr lessThanEquals Expr
// Expr ::= Expr greaterThanEquals Expr
// Expr ::= Expr notEquals Expr
// Expr ::= Expr leftAngle Expr
// Expr ::= Expr rightAngle Expr
/* Notice that for relational operators we use just one parse
   function.  This shows another possible means for implementing
   expressions, as opposed to the method used for arithmetic
   expressions in which each operation has its own parse method.  It
   will depend on what we do in iteration 3 in building an abstract
   syntax tree to decide which method is better.
 */


ParseResult Parser::parseRelationalExpr (ParseResult prLeft)
{
    // parser has already matched left expression 
    ParseResult pr;

    nextToken();
    // just advance token, since examining it in parseExpr caused
    // this method being called.
    string str = prevToken->lexeme;

    pr = parseExpr(prevToken->lbp()); 

	Expr *left = NULL, *right = NULL;
	if(prLeft.ast && pr.ast)
	{
		left = dynamic_cast<Expr*>(prLeft.ast);
		if(!left)throw((string) "Failed cast: expr in parseRelationalExpr");

		right = dynamic_cast<Expr*>(pr.ast);
		if(!right)throw((string) "Failed cast: expr in parseRelationalExpr");
	} 
    pr.ast = new Relation(left , str, right);
    return pr;
}


// Helper function used by the parser.
void Parser::match (tokenType tt) 
{
    if (!attemptMatch(tt)) 
    {
        throw (makeErrorMsgExpected (tt));
    }
}

bool Parser::attemptMatch (tokenType tt) 
{
    if (currToken->terminal == tt) 
    { 
        nextToken();
        return true;
    }
    return false;
}

bool Parser::nextIs (tokenType tt) 
{
    return currToken->terminal == tt;
}

void Parser::nextToken() 
{
    if (currToken == NULL)
    {
        throw (string("Internal Error: should not call nextToken in unitialized state"));
    }
    else if (currToken->terminal == endOfFile && currToken->next == NULL) 
    {
        prevToken = currToken;
    } 
    else if (currToken->terminal != endOfFile && currToken->next == NULL) 
    {
        throw (makeErrorMsg ("Error: tokens end with endOfFile"));
    } 
    else 
    {
        prevToken = currToken;
        currToken = currToken->next;
    }
}

string Parser::terminalDescription (tokenType terminal) 
{
    Token *dummyToken = new Token ("",terminal, NULL);
    ExtToken *dummyExtToken = extendToken (this, dummyToken);
    string s = dummyExtToken->description();
    delete dummyToken;
    delete dummyExtToken;
    return s;
}

string Parser::makeErrorMsgExpected (tokenType terminal) 
{
    string s = (string) "Expected " + terminalDescription (terminal) + " but found " + currToken->description();
    return s;
}

string Parser::makeErrorMsg (tokenType terminal) {
    string s = "Unexpected symbol " + terminalDescription (terminal);
    return s;
}

string Parser::makeErrorMsg (const char *msg) {
    return msg;
}

