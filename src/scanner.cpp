#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#include "regex.h"
#include "scanner.h"

using namespace std; 
bool firstRun = true;
    //Woohoo regular (is it just me or does regex not look regular) expressions
    regex_t *whiteSpace;
    regex_t *blockComment;
    regex_t *lineComment;
    regex_t *_intKwd;
    regex_t *_floatKwd;
    regex_t *_boolKwd;
    regex_t *_trueKwd;
    regex_t *_falseKwd;
    regex_t *_stringKwd;
    regex_t *_matrixKwd;
    regex_t *_letKwd;
    regex_t *_inKwd;
    regex_t *_endKwd;
    regex_t *_ifKwd;
    regex_t *_thenKwd;
    regex_t *_elseKwd;
    regex_t *_forKwd;
    regex_t *_whileKwd;
    regex_t *_printKwd;
    //Constants
    regex_t *_intConst ;
    regex_t *_floatConst ;
    regex_t *_stringConst ;
    //Names
    regex_t *_variableName ;
    //Punctuation
    regex_t *_leftParen;
    regex_t *_rightParen;
    regex_t *_leftCurly;
    regex_t *_rightCurly;
    regex_t *_leftSquare;
    regex_t *_rightSquare;
    regex_t *_comma;
    regex_t *_semicolon;
    regex_t *_colon;
    //Operators
    regex_t *_assign;
    regex_t *_plusSign;
    regex_t *_star;
    regex_t *_dash;
    regex_t *_forwardSlash;
    regex_t *_lessThan;
    regex_t *_lessThanEqual;
    regex_t *_greaterThan;
    regex_t *_greaterThanEqual;
    regex_t *_equalsEquals;
    regex_t *_notEquals;
    regex_t *_andOp;
    regex_t *_orOp;
    regex_t *_notOp;

Token::Token(){}

Token::Token(const char* l, tokenType t, Token* n)
{
    terminal = t;
    lexeme = l; 
    next = n; 
}

int Scanner::consumeWhiteSpaceAndComments (regex_t *whiteSpace, regex_t *blockComment, regex_t *lineComment, const char *text)
{
    int numMatchedChars = 0;
    int totalNumMatchedChars = 0;
    int stillConsumingWhiteSpace;

    do
    {
        stillConsumingWhiteSpace = 0;

        //Attempt to match white space
        numMatchedChars = matchRegex(whiteSpace, text);
        totalNumMatchedChars += numMatchedChars;
        if(numMatchedChars > 0)
        {
            text = text + numMatchedChars;
            stillConsumingWhiteSpace = 1;
        }

        //Attempt to match block comment
        numMatchedChars = matchRegex(blockComment, text);
        totalNumMatchedChars += numMatchedChars;
        if(numMatchedChars > 0)
        {
            text = text + numMatchedChars;
            stillConsumingWhiteSpace = 1;
        }

        //Attemp to match line comments
        numMatchedChars = matchRegex(lineComment, text);
        totalNumMatchedChars += numMatchedChars;
        if(numMatchedChars > 0)
        {
            text = text + numMatchedChars;
            stillConsumingWhiteSpace = 1;
        }
    }
    while(stillConsumingWhiteSpace);
    return totalNumMatchedChars;
}

void Scanner::compileRegex()
{
    //Woohoo regular (is it just me or does regex not look regular) expressions
    whiteSpace = makeRegex("^[\n\t\r ]+") ;
    blockComment = makeRegex("^/\\*([^\\*]|\\*+[^\\*/])*\\*+/");
    lineComment = makeRegex("^//[^\n]*\n");
    _intKwd = makeRegex ("^Int");
    _floatKwd = makeRegex("^Float");
    _boolKwd = makeRegex("^Bool");
    _trueKwd = makeRegex("^True");
    _falseKwd = makeRegex("^False");
    _stringKwd = makeRegex("^Str");
    _matrixKwd = makeRegex("^Matrix");
    _letKwd = makeRegex("^let");
    _inKwd = makeRegex("^in");
    _endKwd = makeRegex("^end");
    _ifKwd = makeRegex("^if");
    _thenKwd = makeRegex("^then");
    _elseKwd = makeRegex("^else");
    _forKwd = makeRegex("^for");
    _whileKwd = makeRegex("^while");
    _printKwd = makeRegex("^print");
    //Constants
    _intConst = makeRegex ("^[0-9]+") ;
    _floatConst = makeRegex ("^[0-9]+\\.[0-9]+");
    _stringConst = makeRegex ("^\"(\\.|[^\"])*\"");
    //Names
    _variableName = makeRegex ("^([a-zA-Z_]+)[0-9]*");
    //Punctuation
    _leftParen = makeRegex("^\\(");
    _rightParen = makeRegex("^\\)");
    _leftCurly = makeRegex("^\\{");
    _rightCurly = makeRegex("^\\}");
    _leftSquare = makeRegex("^\\[");
    _rightSquare = makeRegex("^\\]");
    _comma = makeRegex("^,");
    _semicolon = makeRegex("^;");
    _colon = makeRegex("^:");
    //Operators
    _assign = makeRegex("^=");
    _plusSign = makeRegex("^\\+");
    _star = makeRegex("^\\*");
    _dash = makeRegex("^-");
    _forwardSlash = makeRegex("^/");
    _lessThan = makeRegex("^<");
    _lessThanEqual = makeRegex("^<=");
    _greaterThan = makeRegex("^>");
    _greaterThanEqual = makeRegex("^>=");
    _equalsEquals = makeRegex("^==");
    _notEquals = makeRegex("^!=");
    _andOp = makeRegex("^\\&&");
    _orOp = makeRegex("^\\|\\|");
    _notOp = makeRegex("^!");
}



Token* Scanner::scan (const char * text)
{
    //Set up tokens for use as we go
    Token *firstToken, *currentToken;
    currentToken = new Token();
    firstToken = currentToken;

    if(firstRun == true) //Compile regex only the first time through
    {
        compileRegex();
        firstRun = false;
    }

    // Set up required trackers before entering scan loop
    tokenType matchedType;
    int numMatchedChars = 0;
    numMatchedChars = consumeWhiteSpaceAndComments(whiteSpace, blockComment, lineComment, text);
    text = text + numMatchedChars;
    int MostMatchedChars = 0; //Used to verify matches longest regex possible

    while(text[0] != '\0') //Determines the tokenType and sets it with all required data
    {                    
        numMatchedChars = consumeWhiteSpaceAndComments(whiteSpace, blockComment, lineComment, text);
        MostMatchedChars = 0;
        matchedType = lexicalError;


        ////////////////////////
        //    Match checks    //
        ////////////////////////   

        //Var name
        numMatchedChars = matchRegex(_variableName, text);
        if(numMatchedChars > MostMatchedChars)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = variableName; 
        }

        //Int Const
        numMatchedChars = matchRegex(_intConst, text);
        if(numMatchedChars > MostMatchedChars)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = intConst;
        }

        //Float Const
        numMatchedChars = matchRegex(_floatConst, text);
        if(numMatchedChars > MostMatchedChars)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = floatConst;
        }

        //String Const
        numMatchedChars = matchRegex(_stringConst, text);
        if(numMatchedChars > MostMatchedChars)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = stringConst;
        }

        //IntKwd
        numMatchedChars = matchRegex(_intKwd, text);
        if(numMatchedChars == 3 && MostMatchedChars == 3 && matchedType == variableName)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = intKwd;
        }

        //floatKwd
        numMatchedChars = matchRegex(_floatKwd, text);
        if(numMatchedChars == 5 && MostMatchedChars == 5 && matchedType == variableName)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = floatKwd; 
        }

        numMatchedChars = matchRegex(_boolKwd, text);
        if(numMatchedChars == 4 && MostMatchedChars == 4 && matchedType == variableName)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = boolKwd; 
        }

        numMatchedChars = matchRegex(_trueKwd, text);
        if(numMatchedChars == 4 && MostMatchedChars == 4 && matchedType == variableName)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = trueKwd; 
        }

        numMatchedChars = matchRegex(_falseKwd, text);
        if(numMatchedChars == 5 && MostMatchedChars == 5 && matchedType == variableName)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = falseKwd; 
        }

        //strigKwd
        numMatchedChars = matchRegex(_stringKwd, text);
        if(numMatchedChars == 3 && MostMatchedChars == 3 && matchedType == variableName)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = stringKwd;
        }

        //matrixKwd
        numMatchedChars = matchRegex(_matrixKwd, text);
        if(numMatchedChars == 6 && MostMatchedChars == 6 && matchedType == variableName)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = matrixKwd;
        }

        //letKwd
        numMatchedChars = matchRegex(_letKwd, text);
        if(numMatchedChars == 3 && MostMatchedChars == 3 && matchedType == variableName)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = letKwd;
        }

        //inKwd
        numMatchedChars = matchRegex(_inKwd, text);
        if(numMatchedChars == 2 && MostMatchedChars == 2 && matchedType == variableName)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = inKwd;
        }

        //endKwd
        numMatchedChars = matchRegex(_endKwd, text);
        if(numMatchedChars == 3 && MostMatchedChars == 3 && matchedType == variableName)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = endKwd;
        }

        //ifKwd
        numMatchedChars = matchRegex(_ifKwd, text);
        if(numMatchedChars == 2 && MostMatchedChars == 2 && matchedType == variableName)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = ifKwd;
        }

        //thenKwd
        numMatchedChars = matchRegex(_thenKwd, text);
        if(numMatchedChars == 4 && MostMatchedChars == 4 && matchedType == variableName)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = thenKwd;
        }

        //elseKwd
        numMatchedChars = matchRegex(_elseKwd, text);
        if(numMatchedChars == 4 && MostMatchedChars == 4 && matchedType == variableName)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = elseKwd;
        }

        //forKwd
        numMatchedChars = matchRegex(_forKwd, text);
        if(numMatchedChars == 3 && MostMatchedChars == 3 && matchedType == variableName)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = forKwd;
        }

        //whileKwd
        numMatchedChars = matchRegex(_whileKwd, text);
        if(numMatchedChars == 5 && MostMatchedChars == 5 && matchedType == variableName)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = whileKwd;
        }

        //printKwd
        numMatchedChars = matchRegex(_printKwd, text);
        if(numMatchedChars == 5 && MostMatchedChars == 5 && matchedType == variableName)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = printKwd;
        }

        //leftParen
        numMatchedChars = matchRegex(_leftParen, text);
        if(numMatchedChars == 1)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = leftParen;
        }

        //rightParen
        numMatchedChars = matchRegex(_rightParen, text);
        if(numMatchedChars == 1)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = rightParen;
        }

        //leftCurly
        numMatchedChars = matchRegex(_leftCurly, text);
        if(numMatchedChars == 1)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = leftCurly;
        }

        //rightCurly 
        numMatchedChars = matchRegex(_rightCurly, text);
        if(numMatchedChars == 1)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = rightCurly;
        }

        //leftSquare
        numMatchedChars = matchRegex(_leftSquare, text);
        if(numMatchedChars == 1)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = leftSquare;
        }

        //rightSquare
        numMatchedChars = matchRegex(_rightSquare, text);
        if(numMatchedChars == 1)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = rightSquare;
        }

        //comma
        numMatchedChars = matchRegex(_comma, text);
        if(numMatchedChars == 1)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = comma;
        }

        //semiColon
        numMatchedChars = matchRegex(_semicolon, text);
        if(numMatchedChars == 1)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = semiColon;
        }

        //colon
        numMatchedChars = matchRegex(_colon, text);
        if(numMatchedChars == 1)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = colon;
        }

        //assign
        numMatchedChars = matchRegex(_assign, text);
        if(numMatchedChars == 1)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = assign;
        }

        //plusSign
        numMatchedChars = matchRegex(_plusSign, text);
        if(numMatchedChars == 1)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = plusSign;
        }

        //star
        numMatchedChars = matchRegex(_star, text);
        if(numMatchedChars == 1)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = star;
        }

        //dash
        numMatchedChars = matchRegex(_dash, text);
        if(numMatchedChars == 1)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = dash;
        }

        //forwardSlash
        numMatchedChars = matchRegex(_forwardSlash, text);
        if(numMatchedChars == 1)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = forwardSlash;
        }

        //lessThan
        numMatchedChars = matchRegex(_lessThan, text);
        if(numMatchedChars == 1)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = lessThan;
        }

        //lessThanEqual
        numMatchedChars = matchRegex(_lessThanEqual, text);
        if(numMatchedChars == 2)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = lessThanEqual;
        }

        //greaterThan
        numMatchedChars = matchRegex(_greaterThan, text);
        if(numMatchedChars == 1)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = greaterThan;
        }

        //greaterThanEqual
        numMatchedChars = matchRegex(_greaterThanEqual, text);
        if(numMatchedChars == 2)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = greaterThanEqual;
        }

        //equalsEquals
        numMatchedChars = matchRegex(_equalsEquals, text);
        if(numMatchedChars == 2)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = equalsEquals;
        }

        //notEquals
        numMatchedChars = matchRegex(_notEquals, text);
        if(numMatchedChars == 2)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = notEquals;
        }

        //andOp
        numMatchedChars = matchRegex(_andOp, text);
        if(numMatchedChars == 2)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = andOp;
        }

        //orOp
        numMatchedChars = matchRegex(_orOp, text);
        if(numMatchedChars == 2)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = orOp;
        }

        //notOp
        numMatchedChars = matchRegex(_notOp, text);
        if(numMatchedChars == 1 && matchedType != notEquals)
        {
            MostMatchedChars = numMatchedChars;
            matchedType = notOp;
        }

        //Nothing was matched, skipping over this char
        if(matchedType == lexicalError)
        {
            currentToken->terminal = lexicalError;
            currentToken->lexeme = text[0];
            currentToken->next = new Token();
            currentToken = currentToken->next;
            text = text + 1;
        }
        else //Something was matched set data now 
        {
            currentToken->terminal = matchedType;
            char sub[MostMatchedChars + 1];
            memcpy(sub, text, MostMatchedChars);
            sub[MostMatchedChars] = '\0';
            currentToken->lexeme = sub;
            currentToken->next = new Token();
            currentToken = currentToken->next;
            text = text + MostMatchedChars;
        }

        //Consume whiteSpace and start the loop over with next text chunk
        numMatchedChars = consumeWhiteSpaceAndComments (whiteSpace, blockComment, lineComment, text) ;
        text = text + numMatchedChars;
    }

     if(currentToken->terminal != lexicalError)
     {
        currentToken->terminal = endOfFile;
        currentToken->next = NULL;
     }

    return firstToken;
}