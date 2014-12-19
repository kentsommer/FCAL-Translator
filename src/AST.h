#ifndef Node_H
#define Node_H

#include <string>
#include <iostream>
#include <stdio.h>
#include <sstream> 

#include "scanner.h"

class Node ;

/*! This is the super abstract class for objects that are used to fill the ast field of ParseResult.
 Program, Expr, Decl, Stmts, Stmt are derived from Node.
*/
class Node {
	public:
		//!Pure virtual function
		virtual std::string unparse ( ) = 0;
		//!Virtual function
		virtual std::string cppCode ( ) { return " This should be pure virtual" ; } ;
			//virtual std::string cppCode ( ) = 0;
		//!Virtual deconstructor 
		virtual ~Node() { }
} ;

/*! Subclass of Node. This is a super abstract class for objects used
  to fill the ast field of ParseResult. Tprogram is derived from Program.
*/
class Program : public Node { };


/*! Subclass of Node. This is a super abstract class for objects used
  to fill the ast field of ParseResult.
*/
class Expr : public Node { };


/*! Subclass of Node. This is a super abstract class for objects used
  to fill the ast field of ParseResult.
*/
class Decl : public Node { };


/*! Subclass of Node. This is a super abstract class for objects used
  to fill the ast field of ParseResult.
*/
class Stmts : public Node { };


/*! Subclass of Node. This is a super abstract class for objects used
  to fill the ast field of ParseResult.
*/
class Stmt : public Node { };

/*! This class represents the main program */
class Tprogram : public Program
{
public:
  	/*! Tprogram constructor requires two parameters: function name and Stmts pointer. 
		\param name the function name 'main'
		\param *sstmnts the pointer to Stmts object
  	*/
	Tprogram(string name, Stmts *sstmnts)
	{
		varName = name;
		stmts = sstmnts; 
	}

	/*! Deconstructor for Tprogram*/
	~Tprogram()
	{
		delete[] stmts; 
	}

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
	std::string unparse()
	{
		std::stringstream ss;
		ss << varName << "(){\n" << stmts->unparse() << "\n}"; 
		return ss.str();
	}

	std::string cppCode()
	{
		std::stringstream ss;
		ss << "#include <iostream>\n#include \"Matrix.h\"\n#include <math.h>\nusing namespace std;\nint " <<
			varName << "( ) {\n" << stmts->cppCode() << "\n}";
		return ss.str();
	}

private:
	string varName;
	Stmts *stmts;
	Tprogram() : varName(NULL), stmts(NULL){};
	Tprogram(const Tprogram &) {};
};


/*! This class represents the Constant Expressions */
class Constant : public Expr
{
public:
  	/*! This constructor requires one parameter: the constant.
  		\param s the constant.
  	*/
	Constant(string s)
	{
		constant = s;
	}

	/*! Deconstructor for Constant */
	~Constant(){}

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
	std::string unparse()
	{
		std::stringstream ss;
		ss << constant;
		return ss.str();
	}

  	std::string cppCode() 
  	{
		std::stringstream ss;
		ss << constant;
		return ss.str();
  	}

private:
	string constant;
	Constant() : constant(NULL){};
	Constant(const Constant &){};
};


/*! This class represents the Bool Expressions */
class Bool : public Expr
{
public:
	/*! Constructor for Bool, one parameter
		\param s the Bool
	*/
	Bool(string s)
	{
		boolc = s;
	}

	/*! Deconstructor for Bool */
	~Bool(){}

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
	std::string unparse()
	{
		std::stringstream ss;
		ss << boolc;
		return ss.str();
	}

	std::string cppCode()
	{
		std::stringstream ss;
		if(boolc == "True")
		{
			ss << "true";
		}
		if(boolc == "False")
		{
			ss << "false";
		}
		return ss.str();
	}

private:
	string boolc;
	Bool() : boolc(NULL){};
	Bool(const Bool &){};
};


/*! This class represents the Addition operator Expression */
class Addition : public Expr
{
public:
	/*! Constructor for Addition, two parameters
		\param ex1 the left integer/float
		\param ex2 the right integer/float
	*/
	Addition(Expr *ex1, Expr *ex2)
	{
		expr1 = ex1;
		op2 = ex2; 
	}

	/*! Deconstructor for Addition */
	~Addition()
	{
		delete[] expr1;
		delete[] op2;
	}

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
	std::string unparse()
	{
		std::stringstream ss;
		ss << expr1->unparse() << " + " << op2->unparse();
		return ss.str();
	}

	std::string cppCode() 
	{
    	std::stringstream ss;
    	ss << "(" << expr1->cppCode() << " + " << op2->cppCode() << ")";
    	return ss.str();
  	}

private:
	Expr *expr1;
	Expr *op2;
	Addition() : expr1(NULL), op2(NULL){};
	Addition(const Addition &){};
};



/*! This class represents the Subtraction operator Expression */
class Subtraction : public Expr
{
public:
	/*! Constructor for Subtraction, two parameters
		\param ex1 the left integer/float
		\param ex2 the right integer/float
	*/
	Subtraction(Expr *ex1, Expr *ex2)
	{
		expr1 = ex1;
		op2 = ex2; 
	}

	/*! Deconstructor for Subtraction */
	~Subtraction()
	{
		delete[] expr1;
		delete[] op2;
	}

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
	std::string unparse()
	{
		std::stringstream ss;
		ss << expr1->unparse() << " - " << op2->unparse();
		return ss.str();
	}

	std::string cppCode() 
	{
    	std::stringstream ss;
    	ss << "(" << expr1->cppCode() << " - " << op2->cppCode() << ")";
    	return ss.str();
  	}

private:
	Expr *expr1;
	Expr *op2;
	Subtraction() : expr1(NULL), op2(NULL){};
	Subtraction(const Subtraction &){};
};


/*! This class represents the Multiplication operator Expression */
class Multiplication : public Expr
{
public:
	/*! Constructor for Multiplication, two parameters
		\param ex1 the left integer/float
		\param ex2 the right integer/float
	*/
	Multiplication(Expr *ex1, Expr *ex2)
	{
		expr1 = ex1;
		op2 = ex2; 
	}

	/*! Deconstructor for Multiplication */
	~Multiplication()
	{
		delete[] expr1;
		delete[] op2;
	}

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
	std::string unparse()
	{
		std::stringstream ss;
		ss << expr1->unparse() << " * " << op2->unparse();
		return ss.str();
	}

	std::string cppCode() 
	{
    	std::stringstream ss;
    	ss << "(" << expr1->cppCode() << " * " << op2->cppCode() << ")";
    	return ss.str();
  	}

private:
	Expr *expr1;
	Expr *op2;
	Multiplication() : expr1(NULL), op2(NULL){};
	Multiplication(const Multiplication &){};
};



/*! This class represents the Division operator Expression */
class Division : public Expr
{
public:
	/*! Constructor for Division, two parameters
		\param ex1 the left integer/float
		\param ex2 the right integer/float
	*/
	Division(Expr *ex1, Expr *ex2)
	{
		expr1 = ex1;
		op2 = ex2; 
	}

	/*! Deconstructor for Division */
	~Division()
	{
		delete[] expr1;
		delete[] op2;
	}

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
	std::string unparse()
	{
		std::stringstream ss;
		ss << "\t" << expr1->unparse() << " / " << op2->unparse();
		return ss.str();
	}

	std::string cppCode() 
	{
    	std::stringstream ss;
    	ss << "(" << expr1->cppCode() << " / " << op2->cppCode() << ")";
    	return ss.str();
  	}

private:
	Expr *expr1;
	Expr *op2;
	Division() : expr1(NULL), op2(NULL){};
	Division(const Division &){};
};


/*! This class represents a Relation Expression */
class Relation : public Expr
{
public:
	/*! Constructor for Relation, three parameters 
		\param Expr* ex1 the left integer/float
		\param string op: relation
		\param ex2: right integer/float
	*/
	Relation(Expr *ex1, string op, Expr *ex2)
	{
		expr1 = ex1;
		sop = op; 
		expr2 = ex2; 
	}

	/*! Deconstructor for Relation */
	~Relation()
	{
		delete[] expr1;
		delete[] expr2;
	}

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
	std::string unparse()
	{
		std::stringstream ss;
		ss << "\t" << expr1->unparse() << " " << sop << " " << expr2->unparse();
		return ss.str();
	}

	std::string cppCode() 
	{
    	std::stringstream ss;
    	ss << "(" << expr1->cppCode() << " " << sop << " " << expr2->cppCode() << ")";
    	return ss.str();
  	}

private:
	Expr *expr1;
	Expr *expr2;
	string sop;
	Relation() : expr1(NULL), expr2(NULL), sop(NULL){};
	Relation(const Relation &){};
};


/*! This class represents a VarName Expression */
class VarName : public Expr 
{
public:
	/*! Constructor for VarName, one parameter
		\param name the variable name
	*/
	VarName(string name)
	{
		var = name;
	}

	/*! Deconstructor for VarName */
	~VarName(){}

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
	std::string unparse() 
	{
		std::stringstream ss;
		ss << var;
		return ss.str();
	}

	std::string cppCode() 
	{
		std::stringstream ss;
		ss << var;
		return ss.str();
	}

private:
	string var;
	VarName():var(NULL){};
	VarName(const VarName &){};
};


/*! This class represents a VarNameMatrix Expression */
class VarNameMatrix : public Expr
{
public:
	/*! Constructor for VarNameMatrix, three parameters 
		\param name the Matrix
		\param ex1 the left integer/float inside brackets
		\param ex2 the right integer/float inside brackets
	*/
	VarNameMatrix(string name, Expr *ex1, Expr *ex2)
	{
		varName = name;
		expr1 = ex1;
		expr2 = ex2;
	}

	/*! Deconstructor for VarNameMatrix */
	~VarNameMatrix()
	{
		delete[] expr1;
		delete[] expr2;
	}

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
	std::string unparse()
	{
		std::stringstream ss;
		ss << varName << "[" << expr1->unparse() << ", " << expr2->unparse() << "]";
		return ss.str();
	}

  	std::string cppCode() 
  	{
    	std::stringstream ss;
    	ss << "*(" << varName << ".access(" << expr1->cppCode() << ", " << expr2->cppCode() << "))";
    	return ss.str();
  	}

private:
	string varName;
	Expr *expr1;
	Expr *expr2;
	VarNameMatrix() : varName(NULL), expr1(NULL), expr2(NULL){};
	VarNameMatrix(const VarNameMatrix &){};
};

/*! This class represents a VarNameNested Expression*/
class VarNameNested : public Expr
{
public:
	/*! Constructor for VarNameNested, two parameters 
		\param name the variable name
		\param ex1 the nexted expression 
	*/
	VarNameNested(string name, Expr *ex1)
	{
		varName = name;
		expr1 = ex1;
	}

	/*! Deconstructor for VarNameNested */
	~VarNameNested()
	{
		delete[] expr1;
	}

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
	std::string unparse()
	{
		std::stringstream ss;
		ss<< varName << "(" << expr1->unparse() << ")";
		return ss.str();
	}

	std::string cppCode()
	{
		std::stringstream ss;
		ss<< varName << "(" << expr1->cppCode() << ")";
		return ss.str();
	}

private:
	string varName;
	Expr *expr1;
	VarNameNested() : varName(NULL), expr1(NULL){};
	VarNameNested(const VarNameNested &){};
};


/*! This class represents a ExprExpr Expression */
class ExprExpr : public Expr
{
public:
	/*! Constructor for ExprExpr, one parameter
		\param ex1 the nested expression 
	*/
	ExprExpr(Expr *ex1)
	{
		expr1 = ex1; 
	}

	/*! Deconstructor for ExprExpr */
	~ExprExpr()
	{
		delete[] expr1;
	}

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
	std::string unparse()
	{
		std::stringstream ss;
		ss << "(" << expr1->unparse() << ")";
		return ss.str();
	}

	std::string cppCode()
	{
		std::stringstream ss;
		ss << "(" << expr1->unparse() << ")";
		return ss.str();
	}

private:
	Expr *expr1;
	ExprExpr() : expr1(NULL){};
	ExprExpr(const ExprExpr &){};
};


/*! This class represents a LetExpr Expression */
class LetExpr : public Expr
{
public:
	/*! Constructor for LetExpr, two parameters 
		\param sstmnts1 the statements
		\param ex1 the expression
	*/
	LetExpr(Stmts* sstmnts1, Expr *ex1)
	{
		stmts = sstmnts1;
		expr1 = ex1;
	}

	/*! Deconstructor for LetExpr */
	~LetExpr()
	{
		delete[] stmts;
		delete[] expr1;
	}

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
	std::string unparse()
	{
		std::stringstream ss;
		ss << "let " << stmts->unparse() << " in " << expr1->unparse() << " end";
		return ss.str();
	}

	std::string cppCode()
	{
    	std::stringstream ss;
    	ss << "({\n" << stmts->cppCode() << "\n\t" << expr1->cppCode() << ";\n\t})";
    	return ss.str();
  	}

private:
	Stmts *stmts;
	Expr *expr1;
	LetExpr() : stmts(NULL), expr1(NULL){};
	LetExpr(const LetExpr &) {};
};


/*! This class represents a IfExpr Expression */
class IfExpr : public Expr
{
public:
	/*! Constructor for IfExpr, three parameters 
		\param ex1 the expression after "if"
		\param ex2 the expression after "then"
		\param ex3 the expression after "else"
	*/
	IfExpr(Expr *ex1, Expr *ex2, Expr *ex3)
	{
		expr1 = ex1;
		expr2 = ex2;
		expr3 = ex3;
	}

	/*! Deconstructor for IfExpr */
	~IfExpr()
	{
		delete[] expr1;
		delete[] expr2;
		delete[] expr3;
	}

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
	std::string unparse()
	{
		std::stringstream ss;
		ss << "if " << expr1->unparse() << "\n" << "then " << expr2->unparse() << "\n" << "else " << expr3->unparse();
		return ss.str();
	}

	std::string cppCode()
	{
    	std::stringstream ss;
    	ss << "(" << expr1->cppCode() << " ? " << expr2->cppCode() << " : "
       		<< expr3->cppCode() << ")";
    	return ss.str();
  	}

private:
	Expr *expr1;
	Expr *expr2;
	Expr *expr3;
	IfExpr() : expr1(NULL), expr2(NULL), expr3(NULL){};
	IfExpr(const IfExpr &) {};
};


/*! This class represents a NotExpr Expression */
class NotExpr : public Expr
{
public:
	/*! Constructor for NotExpr, one parameter
		\param ex1 the expression after "!"
	*/
	NotExpr(Expr *ex1)
	{
		expr1 = ex1;
	}

	/*! Deconstructor for NotExpr */
	~NotExpr()
	{
		delete[] expr1;
	}

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
	std::string unparse()
	{
		std::stringstream ss;
		ss << "!" << expr1->unparse();
		return ss.str();
	}

	std::string cppCode()
	{
		std::stringstream ss;
		ss << "!" << expr1->cppCode();
		return ss.str();
	}

private:
	Expr *expr1;
	NotExpr() : expr1(NULL){};
	NotExpr(const IfExpr &) {};
};


/*! This class represents a StandardDecl Declaration*/
class StandardDecl : public Decl
{
public:
	/*! Constructor for StandardDecl, two parameters 
		\param x the data type
		\param name the variable name 
	*/
	StandardDecl(string x, string name)
	{
		kwd = x; 
		varName = name;
	}

	/*! Deconstructor for StandardDecl */
	~StandardDecl(){}

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
	std::string unparse()
	{
		std::stringstream ss;
		ss << kwd << " " << varName << ";";
		return ss.str();
	}

	std::string cppCode()
	{
    	std::stringstream ss;
		if(kwd.compare("Int") == 0)
		{
			kwd = "int";
		}
		else if(kwd.compare("Float") == 0)
		{
			kwd = "float";
		}
		else if(kwd.compare("String") == 0)
		{
			kwd = "string";
		}
		else if(kwd.compare("Bool") == 0)
		{
			kwd = "bool";
		}
    	ss << kwd << " " << varName << ";";
    	return ss.str();
  }

private:
	string kwd;
	string varName;
	StandardDecl() : kwd(NULL), varName(NULL){};
	StandardDecl(const StandardDecl &){};
};


/*! This class represents a MatrixDecl Declaration */
class MatrixDecl : public Decl
{
public:
	/*! Constructor for MatrixDecl, six parameters 
		\param str1 the name of matrix 
		\param ex1 the expression used in matrix
		\param ex2 the expression used in matrix 
		\param str2 the name of variable
		\param str3 the name of variable 
		\param ex3 the expression assigned to the matrix 
	*/
	MatrixDecl(string str1, Expr *ex1, Expr *ex2, string str2, string str3, Expr *ex3)
	{
		varName1 = str1;
		expr1 = ex1; 
		expr2 = ex2;
		varName2 = str2;
		varName3 = str3; 
		expr3 = ex3;
	}

	/*! Deconstructor for MatrixDecl */
	~MatrixDecl()
	{
		delete[] expr1;
		delete[] expr2;
		delete[] expr3;
	}

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
	std::string unparse()
	{
		std::stringstream ss;
		ss << "Matrix " << varName1 << "[" << expr1->unparse() << ", " << expr2->unparse() << "] "
			<< varName2 << ", " << varName3 << " = " << expr3->unparse() << ";";
		return ss.str();
	}

	std::string cppCode()
	{
    	std::stringstream ss;
    	ss << "Matrix " << varName1 << "(" << expr1->cppCode() << ", " << expr2->cppCode() << ");\n"
       		<< "for (int " << varName2 << " = 0; " << varName2 << " < " << expr1->cppCode() << "; "
       		<< varName2 << "++){\n\t" 
       		<< "for (int " << varName3 << " = 0; " << varName3 << " < " << expr2->cppCode() << "; "
       		<< varName3 << "++){\n\t\t" << "*(" << varName1 << ".access(" << varName2 << ", " << varName3
       		<< ")) = " << expr3->cppCode() << ";\n}\n}";  
    	return ss.str();
  }

private:
	string varName1;
	Expr *expr1;
	Expr *expr2;
	string varName2;
	string varName3;
	Expr *expr3;
	MatrixDecl() : varName1(NULL), expr1(NULL), expr2(NULL), varName2(NULL), varName3(NULL), expr3(NULL){};
	MatrixDecl(const MatrixDecl &){};
};


/*! This class represents a AssignMatrixDecl Declaration */
class AssignMatrixDecl : public Decl
{
public:
	/*! Constructor for AssignMatrixDecl, two parameters 
		\param str1 the matrix name 
		\param ex1 the value assigned to matrix variable
	*/
	AssignMatrixDecl(string str1, Expr *ex1)
	{
		varName = str1; 
		expr1 = ex1;
	}

	/*! Deconstructor for AssignMatrixDecl */
	~AssignMatrixDecl()
	{
		delete[] expr1;
	}

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
	std::string unparse()
	{
		std::stringstream ss;
		ss << "Matrix " << varName << " = " << expr1->unparse() << ";";
		return ss.str();
	}

	std::string cppCode()
	{
    	std::stringstream ss;
    	ss << "Matrix " << varName << " (Matrix::" << expr1->cppCode() << " );";
    	return ss.str();
  	}

private:
	string varName;
	Expr *expr1;
	AssignMatrixDecl() : varName(NULL), expr1(NULL){};
	AssignMatrixDecl(const AssignMatrixDecl &){};
};


/*! This class represents a DeclStmt Statement */
class DeclStmt : public Stmt
{
public:
	/*! Constructor for DeclStmt, one parameter
		\param dec1 the decl used in derivations of the decleration productions
	*/
	DeclStmt(Decl *dec1)
	{
		decl1 = dec1;
	}

	/*! Deconstructor for DeclStmt */
	~DeclStmt()
	{
		delete[] decl1;
	}

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
	std::string unparse()
	{
		std::stringstream ss;
		ss << decl1->unparse();
		return ss.str();
	}

	std::string cppCode()
	{
		std::stringstream ss;
		ss << "\t" << decl1->cppCode();
		return ss.str();
	}

private:
	Decl *decl1;
	DeclStmt() : decl1(NULL){};
	DeclStmt(const DeclStmt &){};
};


/*! This class represents a StmtsStmt Statement */
class StmtsStmt : public Stmt
{
public:
	/*! Constructor for StmtsStmt, one parameter
		\param sstmts1 the statments object to create more statements
	*/
	StmtsStmt(Stmts *sstmts1)
	{
		stmts1 = sstmts1;
	}

	/*! Deconstructor for StmtsStmt */
	~StmtsStmt()
	{
		delete[] stmts1;
	}

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
	std::string unparse()
	{
		std::stringstream ss;
		ss << "{\n" << stmts1->unparse() << "}\n";
		return ss.str();
	}

	std::string cppCode()
	{
		std::stringstream ss;
		ss << "{\n" << stmts1->cppCode() << "\n}";
		return ss.str();
	}

private:
	Stmts *stmts1;
	StmtsStmt() : stmts1(NULL){};
	StmtsStmt(const StmtsStmt &){};
};


/*! This class represents a IfStmt Statement */
class IfStmt : public Stmt
{
public:
	/*! Constructor for IfStmt, two parameters 
		\param ex1 the expression object for the if statements boolean expression
		\param sstmt1 the statement object for inside if statement
	*/
	IfStmt(Expr *ex1, Stmt *sstmt1)
	{
		expr1 = ex1;
		stmt1 = sstmt1;
	}

	/*! Deconstructor for IfStmt */
	~IfStmt()
	{
		delete[] expr1;
		delete[] stmt1;
	}

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
	std::string unparse()
	{
		std::stringstream ss;
		ss<< "if(" << expr1->unparse() << ")\n\t" << stmt1->unparse();
		return ss.str();
	}

	std::string cppCode()
	{
    	std::stringstream ss;
    	ss << "if" << expr1->cppCode() << "\n\t" << stmt1->cppCode() << "\n";
    	return ss.str();
  	}

private:
	Expr *expr1;
	Stmt *stmt1;
	IfStmt() : expr1(NULL), stmt1(NULL){};
	IfStmt(const IfStmt &){};

};


/*! This class represents a IfElseStmt Statement */
class IfElseStmt : public Stmt{
 public:
 	/*! Constructor for IfElseStmt, three parameters 
		\param ex1 the if statment boolean expression 
		\param sstmt1 the statement for inside the if
		\param sstmt2 the statement for inside the else
	*/
  IfElseStmt(Expr *ex1, Stmt *sstmt1, Stmt *sstmt2)
  {
    expr1 = ex1;
    stmt1 = sstmt1;
    stmt2 = sstmt2;
  }

	/*! Deconstructor for IfElseStmt */
  ~IfElseStmt(){
	delete[] expr1;
	delete[] stmt1;
	delete[] stmt2;
  }

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
  std::string unparse()
  {
    std::stringstream ss;
    ss << "if(" << expr1->unparse() << ")\n\t" << stmt1->unparse() << "\nelse\n\t" << stmt2->unparse();
    return ss.str();
  }

  std::string cppCode()
  {
  	std::stringstream ss;
    ss << "if(" << expr1->cppCode() << "){\n\t" << stmt1->cppCode() << "\n}"
       << "\nelse{\n\t" << stmt2->cppCode() << "\n}";
    return ss.str();
  }

 private:
  Expr *expr1;
  Stmt *stmt1;
  Stmt *stmt2;
  IfElseStmt() : expr1(NULL), stmt1(NULL), stmt2(NULL){};
  IfElseStmt(const IfElseStmt &){};
};


/*! This class represents a AssignStmt Statement */
class AssignStmt : public Stmt
{
public:
	/*! Constructor for AssignStmt, two parameters 
		\param str1 the variable name 
		\param ex1 the expression assigned to variable name 
	*/
	AssignStmt(string str1, Expr *ex1)
	{
		varName = str1; 
		expr1 = ex1;
	}

	/*! Deconstructor for AssignStmt */
	~AssignStmt()
	{
		delete[] expr1;
	}

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
	std::string unparse()
	{
		std::stringstream ss;
		ss << varName << " = " << expr1->unparse() << ";";
		return ss.str();
	}

	std::string cppCode()
	{
		std::stringstream ss;
		ss << "\t" << varName << " = " << expr1->cppCode() << ";";
		return ss.str();
	}

private:
	string varName;
	Expr *expr1;
	AssignStmt() : varName(NULL), expr1(NULL){};
	AssignStmt(const AssignStmt &){};
};


/*! This class represents a AssignmStmt Statement */
class AssignmStmt : public Stmt
{
public:
	/*! Constructor for AssignmStmt, four parameters 
	* string str1: variable name 
		\param ex1 the expression inside brackets 
		\param ex2 the expression inside brackets 
		\param ex3 the expression value assigned to matrix  
	*/
	AssignmStmt(string str1, Expr *ex1, Expr *ex2, Expr *ex3)
	{
		varName = str1;
		expr1 = ex1;
		expr2 = ex2;
		expr3 = ex3;
	}

	/*! Deconstructor for AssignmStmt */
	~AssignmStmt()
	{
		delete[] expr1;
		delete[] expr2;
		delete[] expr3;
	}

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
	std::string unparse()
	{
		std::stringstream ss;
		ss << varName << "[" << expr1->unparse() << ", " << expr2->unparse() << "]"
			<< " = " << expr3->unparse() << ";";
		return ss.str();
	}

	std::string cppCode()
	{
    	std::stringstream ss;
    	ss << "\t" << "*(" << varName << ".access(" << expr1->cppCode() << ", " << expr2->cppCode()
       		<< ")) = " << expr3->cppCode() << ";";
    	return ss.str();
  	}

private:
	string varName;
	Expr *expr1;
	Expr *expr2;
	Expr *expr3;
	AssignmStmt() : varName(NULL), expr1(NULL), expr2(NULL), expr3(NULL){};
	AssignmStmt(const AssignmStmt &){};
};


/*! This class represents a PrintStmt Statement */
class PrintStmt : public Stmt
{
public:
	/*! Constructor for PrintStmt, one parameter 
		\param ex1 the expression used to print
	*/
	PrintStmt(Expr *ex1)
  	{
    	expr1 = ex1;
  	}

	/*! Deconstructor for PrintStmt */
  	~PrintStmt()
  	{
		delete[] expr1;
  	}

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
  	std::string unparse()
  	{
    	std::stringstream ss;
    	ss << "print" << "(" << expr1->unparse() << ")" << ";";
    	return ss.str();
  	}

  	std::string cppCode()
  	{
    	std::stringstream ss;
    	ss << "\tcout << " << expr1->cppCode() << ";";
    	return ss.str();
  	}

private:
	Expr *expr1;
 	PrintStmt() : expr1(NULL){};
  	PrintStmt(const PrintStmt &){};
};


/*! This class represents a ForStmt Statement */
class ForStmt : public Stmt
{
 public:
 	/*! Constructor for ForStmt, four parameters 
		\param str1 the "for" name
		\param ex1 the start for the loop 
		\param ex2 the termination for the loop
		\param ex3 the counter for incrementing
	*/ 
  ForStmt(string str1, Expr *ex1, Expr *ex2, Stmt *sstmt1)
  {
    varName = str1;
    expr1 = ex1;
    expr2 = ex2;
    stmt1 = sstmt1;
  }

	/*! Deconstructor for ForStmt */
  ~ForStmt()
  {
	delete[] expr1;
	delete[] expr2;
	delete[] stmt1;
  }

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
  std::string unparse()
  {
    std::stringstream ss;
    ss << "for" << "(" << varName << " = " << expr1->unparse() << " : " << expr2->unparse() 
       << ")\n" << stmt1->unparse();
    return ss.str();
  }

  std::string cppCode()
  {
    std::stringstream ss;
    ss << "\tfor(" << varName << " = " << expr1->cppCode() << " ; " << varName << " <= "
       << expr2->cppCode() << "; " << varName << " ++ )\n" << stmt1->cppCode() << "\n";
    return ss.str();
  }

 private:
  string varName;
  Expr *expr1;
  Expr *expr2;
  Stmt *stmt1;
  ForStmt() : varName(NULL), expr1(NULL), expr2(NULL), stmt1(NULL){};
  ForStmt(const ForStmt &){};
};


/*! This class represents a WhileStmt Statement */
class WhileStmt : public Stmt
{
 public: 
 	/*! Constructor for WhileStmt, two parameters 
		\param ex1 the expression for boolean of while
		\param sstmt1 the statement inside while loop 
	*/
  WhileStmt(Expr *ex1, Stmt *sstmt1)
  {
    expr1 = ex1;
    stmt1 = sstmt1;
  }

	/*! Deconstructor for WhileStmt */
  ~WhileStmt()
  {
	delete[] expr1;
	delete[] stmt1;
  }

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
  std::string unparse()
  {
    std::stringstream ss;
    ss << "while" << "(" << expr1->unparse() << ")\n" << stmt1->unparse();
    return ss.str();
  }

  std::string cppCode()
  {
    std::stringstream ss;
    ss << "while" << expr1->cppCode() << "\n" << stmt1->cppCode() << "\n";
    return ss.str();
  }

 private:
  Expr *expr1;
  Stmt *stmt1;
  WhileStmt() : expr1(NULL), stmt1(NULL){};
  WhileStmt(const WhileStmt &){};
};


/*! This class represents a ColonStmt Statement */
class ColonStmt : public Stmt
{
 public:
 	/*! Constructor for ColonStmt, no parameters 
	*/
  ColonStmt(){}

	/*! Deconstructor for ColonStmt */
  ~ColonStmt(){}

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
  std::string unparse(){ return ";";}

  std::string cppCode()
  {
    std::stringstream ss;
    ss << ";";
    return ss.str();
  }
};

/*! This class represents NullStmts Statements */
class NullStmts : public Stmts {
 public:
 	/*! Constructor for NullStmts, no parameters 
	*/
  NullStmts() {}

	/*! Deconstructor for NullStmts */
  ~NullStmts(){}

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
  std::string unparse() { return "";}
  std::string cppCode() { return "";}
};


/*! This class represents ConsStmts Statements */
class ConsStmts : public Stmts 
{
 public:
 	/*! Constructor for ConsStmts, two parameters 
		\param sstmt1 the newly created statement
		\param sstmts1 the possible new statements 
	*/
  ConsStmts(Stmt *sstmt1, Stmts *sstmts1) 
  {
    stmt1 = sstmt1;
    stmts1 = sstmts1;
  }

	/*! Deconstructor for ConsStmts */
  ~ConsStmts(){
	delete[] stmt1;
	delete[] stmts1;
  }

	/*! unparse takes no argument and returns a string.
  	* return is: The FCAL string matched in parser.cpp.
    */
  std::string unparse() 
  {
    std::stringstream ss;
    ss << stmt1->unparse() << "\n" << stmts1->unparse() ;
    return ss.str();
  }

  std::string cppCode() 
  {
    std::stringstream str;
    str << stmt1->cppCode() << "\n" << stmts1->cppCode() ;
    return str.str();
  }

 private:
  Stmt *stmt1;
  Stmts *stmts1;
  ConsStmts() : stmt1(NULL), stmts1(NULL){};
  ConsStmts(const ConsStmts &){};
};
#endif
