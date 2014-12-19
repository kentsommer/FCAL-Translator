/* A small meaningless comment */
#ifndef SCANNER_H
#define SCANNER_H

#include <regex.h>
#include <string>

using namespace std; 

class Token ;

/* This enumerated type is used to keep track of what kind of
   construct was matched. 
 */
enum tokenEnumType { 

	intKwd, floatKwd, boolKwd, // 0 - 2
	trueKwd, falseKwd, stringKwd,  matrixKwd, //3 - 6
	letKwd, inKwd, endKwd, ifKwd, thenKwd, elseKwd, // 7 - 12
	forKwd, whileKwd, printKwd, // 13 - 15

	// Constants
	intConst, floatConst, stringConst, // 16 -18

	// Names
	variableName , // 19

	// Punctuation
	leftParen, rightParen, //20 - 21
	leftCurly, rightCurly,  // 22 - 23
	leftSquare, rightSquare, // 24 - 25

	comma, semiColon, colon, // 26 - 28

	//Operators
	assign, // 29
	plusSign, star, dash, forwardSlash, // 3 - 33
	lessThan, lessThanEqual, greaterThan, greaterThanEqual, //34 - 37
	equalsEquals, notEquals, // 38 - 39
	andOp, orOp, notOp, // 40 - 42

	// Special terminal types
	endOfFile , // 43
	lexicalError // 44
} ;
typedef enum tokenEnumType tokenType ;

// Below you need to write your class definitions for Token and Scanner.

class Token 
{
	public:
		Token();
		Token(const char* l, tokenType t, Token* n);
		tokenType terminal;
		string lexeme;
		Token* next;
};

class Scanner
{
	public:
		void compileRegex();
		Token *scan(const char *);
		int consumeWhiteSpaceAndComments(regex_t *whiteSpace, regex_t *blockComment, regex_t *lineComment, const char *text); 
};


#endif /* SCANNER_H */
