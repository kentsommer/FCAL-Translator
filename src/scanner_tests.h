#include <cxxtest/TestSuite.h>

#include "readInput.h"
#include "scanner.h"
#include <stdio.h>
using namespace std ;

class ScannerTestSuite : public CxxTest::TestSuite 
{
public:
    /* A Scanner object is created in the test_terminal_setup_code method.
       This is a test, beginning with "test_terminal_", so that it is executed
       by the testing framework.  The scanner is used in some tests of
       the method "scan".
     */
    Scanner *s ;
    void test_terminal_setup_code ( ) {
        s = new Scanner() ;
    }


    // Tests for components and functions used by "scan"
    // --------------------------------------------------

    /* You may need to write several tests to adequately test the
       functions that are called from "scan".  Once you are confident
       that the components used by "scan" work properly, then you can
       run tests on the "scan" method itself.
    */

    // Here you should place tests for these other functions.


    /* Below is one of the tests for these components in the project
       solution created by your instructor..  It uses a helper
       function function called "tokenMaker_tester", which you have
       not been given.  You are expected to design your own components
       for "scan" and your own mechanisms for easily testing them.

        void xtest_terminal_TokenMaker_leftCurly ( ) {
            tokenMaker_tester ("{ ", "^\\{", leftCurly, "{" ) ;
        }

        Note that this test is here named "xtest_terminal_Token..." The leading
        "x" is so that cxxTest doesn't scan the line above and think
        it is an actual test that isn't commented out.  cxxTest is
        very simple and doesn't even process block comments.
    */


    /* You must have at least one separate test case for each terminal
       symbol.  Thus, you need a test that will pass or fail based
       solely on the regular expression (and its corresponding code)
       for each terminal symbol.

       This requires a test case for each element of the enumerated
       type tokenType.  This may look something like the sample test
       shown in the comment above.
    */

    void test_terminal_intKwd()
    {
        Token *current;
        current = s->scan("Int");
        TS_ASSERT_EQUALS(current->terminal, intKwd);
    }

    void test_terminal_floatKwd()
    {
        Token *current;
        current = s->scan("Float");
        TS_ASSERT_EQUALS(current->terminal, floatKwd);
    }

    void test_terminal_boolKwd()
    {
        Token *current;
        current = s->scan("Bool");
        TS_ASSERT_EQUALS(current->terminal, boolKwd);
    }

    void test_terminal_trueKwd()
    {
        Token *current;
        current = s->scan("True");
        TS_ASSERT_EQUALS(current->terminal, trueKwd);
    }

    void test_terminal_falseKwd()
    {
        Token *current;
        current = s->scan("False");
        TS_ASSERT_EQUALS(current->terminal, falseKwd);
    }

    void test_terminal_stringKwd()
    {
        Token *current;
        current = s->scan("Str");
        TS_ASSERT_EQUALS(current->terminal, stringKwd);
    }

    void test_terminal_matrixKwd()
    {
        Token *current;
        current = s->scan("Matrix");
        TS_ASSERT_EQUALS(current->terminal, matrixKwd);
    }

    void test_terminal_letKwd()
    {
        Token *current;
        current = s->scan("let");
        TS_ASSERT_EQUALS(current->terminal, letKwd);
    }

    void test_terminal_inKwd()
    {
        Token *current;
        current = s->scan("in");
        TS_ASSERT_EQUALS(current->terminal, inKwd);
    }

    void test_terminal_endKwd()
    {
        Token *current;
        current = s->scan("end");
        TS_ASSERT_EQUALS(current->terminal, endKwd);
    }

    void test_terminal_ifKwd()
    {
        Token *current;
        current = s->scan("if");
        TS_ASSERT_EQUALS(current->terminal, ifKwd);
    }

    void test_terminal_thenKwd()
    {
        Token *current;
        current = s->scan("then");
        TS_ASSERT_EQUALS(current->terminal, thenKwd);
    }

    void test_terminal_elseKwd()
    {
        Token *current;
        current = s->scan("else");
        TS_ASSERT_EQUALS(current->terminal, elseKwd);
    }

    void test_terminal_forKwd()
    {
        Token *current;
        current = s->scan("for");
        TS_ASSERT_EQUALS(current->terminal, forKwd);
    }

    void test_terminal_whileKwd()
    {
        Token *current;
        current = s->scan("while");
        TS_ASSERT_EQUALS(current->terminal, whileKwd);
    }

    void test_terminal_printKwd()
    {
        Token *current;
        current = s->scan("print");
        TS_ASSERT_EQUALS(current->terminal, printKwd);
    }

    void test_terminal_intConst()
    {
        Token *current;
        current = s->scan("3148");
        TS_ASSERT_EQUALS(current->terminal, intConst);
    }

    void test_terminal_floatConst()
    {
        Token *current;
        current = s->scan("31.48");
        TS_ASSERT_EQUALS(current->terminal, floatConst);
    }

    void test_terminal_stringConst()
    {
        Token *current;
        current = s->scan("\"Hellow World\"");
        TS_ASSERT_EQUALS(current->terminal, stringConst);
    }

    void test_terminal_variableName()
    {
        Token *current;
        current = s->scan("isThisOk99");
        TS_ASSERT_EQUALS(current->terminal, variableName);
    }

    void test_terminal_leftParen()
    {
        Token *current;
        current = s->scan("(");
        TS_ASSERT_EQUALS(current->terminal, leftParen);
    }

    void test_terminal_rightParen()
    {
        Token *current;
        current = s->scan(")");
        TS_ASSERT_EQUALS(current->terminal, rightParen);
    }

    void test_terminal_leftCurly()
    {
        Token *current;
        current = s->scan("{");
        TS_ASSERT_EQUALS(current->terminal, leftCurly);
    }

    void test_terminal_rightCurly()
    {
        Token *current;
        current = s->scan("}");
        TS_ASSERT_EQUALS(current->terminal, rightCurly);
    }

    void test_terminal_leftSquare()
    {
        Token *current;
        current = s->scan("[");
        TS_ASSERT_EQUALS(current->terminal, leftSquare);
    }

    void test_terminal_rightSquare()
    {
        Token *current;
        current = s->scan("]");
        TS_ASSERT_EQUALS(current->terminal, rightSquare);
    }

    void test_terminal_comma()
    {
        Token *current;
        current = s->scan(",");
        TS_ASSERT_EQUALS(current->terminal, comma);
    }

    void test_terminal_semiColon()
    {
        Token *current;
        current = s->scan(";");
        TS_ASSERT_EQUALS(current->terminal, semiColon);
    }

    void test_terminal_colon()
    {
        Token *current;
        current = s->scan(":");
        TS_ASSERT_EQUALS(current->terminal, colon);
    }

    void test_terminal_assign()
    {
        Token *current;
        current = s->scan("=");
        TS_ASSERT_EQUALS(current->terminal, assign);
    }

    void test_terminal_plusSign()
    {
        Token *current;
        current = s->scan("+");
        TS_ASSERT_EQUALS(current->terminal, plusSign);
    }

    void test_terminal_star()
    {
        Token *current;
        current = s->scan("*");
        TS_ASSERT_EQUALS(current->terminal, star);
    }

    void test_terminal_dash()
    {
        Token *current;
        current = s->scan("-");
        TS_ASSERT_EQUALS(current->terminal, dash);
    }

    void test_terminal_forwardSlash()
    {
        Token *current;
        current = s->scan("/");
        TS_ASSERT_EQUALS(current->terminal, forwardSlash);
    }

    void test_terminal_lessThan()
    {
        Token *current;
        current = s->scan("<");
        TS_ASSERT_EQUALS(current->terminal, lessThan);
    }

    void test_terminal_lessThanEqual()
    {
        Token *current;
        current = s->scan("<=");
        TS_ASSERT_EQUALS(current->terminal, lessThanEqual);
    }

    void test_terminal_greaterThan()
    {
        Token *current;
        current = s->scan(">");
        TS_ASSERT_EQUALS(current->terminal, greaterThan);
    }

    void test_terminal_greaterThanEqual()
    {
        Token *current;
        current = s->scan(">=");
        TS_ASSERT_EQUALS(current->terminal, greaterThanEqual);
    }

    void test_terminal_equalsEquals()
    {
        Token *current;
        current = s->scan("==");
        TS_ASSERT_EQUALS(current->terminal, equalsEquals);
    }

    void test_terminal_notEquals()
    {
        Token *current;
        current = s->scan("!=");
        TS_ASSERT_EQUALS(current->terminal, notEquals);
    }

    void test_terminal_andOp()
    {
        Token *current;
        current = s->scan("&&");
        TS_ASSERT_EQUALS(current->terminal, andOp);
    }

    void test_terminal_orOp()
    {
        Token *current;
        current = s->scan("||");
        TS_ASSERT_EQUALS(current->terminal, orOp);
    }

    void test_terminal_notOp()
    {
        Token *current;
        current = s->scan("!");
        TS_ASSERT_EQUALS(current->terminal, notOp);
    }

    void test_terminal_lexicalError()
    {
        Token *current;
        current = s->scan("&|");
        TS_ASSERT_EQUALS(current->terminal, lexicalError);
    }





    // Tests for "scan"
    // --------------------------------------------------

    /* Below are some helper functions and sample tests for testing the 
       "scan" method on Scanner.
    */

    // Test that a list of tokens has no lexicalError tokens.
    bool noLexicalErrors (Token *tks) {
        Token *currentToken = tks ;
        while (currentToken != NULL) {
            if (currentToken->terminal == lexicalError) {
                printf("priblem: %s\n",currentToken->lexeme.c_str());
                return false ;
            }
            else {
                currentToken = currentToken->next ;
            }
        }
        return true ;
    }

    /* A quick, but inaccurate, test for scanning files.  It only
       checks that no lexical errors occurred, not that the right
       tokens were returned. 
    */
    void scanFileNoLexicalErrors ( const char* filename ) {
        char *text =  readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;
        Token *tks = s->scan ( text ) ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT (noLexicalErrors(tks));
    }


    /* This function checks that the terminal fields in the list of
       tokens matches a list of terminals.
    */
    bool sameTerminals (Token *tks, int numTerms, tokenType *ts) {
        Token *currentToken = tks ;
        int termIndex = 0 ;
        while (currentToken != NULL && termIndex < numTerms ) {
            if (currentToken->terminal != ts[termIndex]) {
            printf("\n%i: %i should be %i\n",termIndex,currentToken->terminal , ts[termIndex]);
            fflush(stdout);
                return false ;
            }
            else {
                ++ termIndex ;
                currentToken = currentToken->next ;
            }
        }
        return true ;
    }


    /* Below are the provided test files that your code should also
       pass.

       You may initially want to rename these tests to "xtest_terminal_..." so
       that the CxxTest framework does not see it as a test and won't
       run it as one.  This way you can focus on the tests that you'll
       write above for the individual terminal types first. Then focus
       on these tests.

    */

    // The "endOfFile" token is always the last one in the list of tokens.
    void test_terminal_scan_empty ( ) {
        Token *tks = s->scan ("  ") ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
        TS_ASSERT (tks->next == NULL) ;
    }

    void test_terminal_scan_empty_comment ( ) {
        Token *tks = s->scan (" /* a comment */ ") ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
        TS_ASSERT (tks->next == NULL) ;
    }

    // When a lexical error occurs, the scanner creates a token with a 
    // single-character lexeme and lexicalError as the terminal.
   void test_terminal_scan_lexicalErrors ( ) {
        Token *tks = s->scan ("$&1  ") ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, lexicalError) ;
        TS_ASSERT_EQUALS (tks->lexeme, "$");
        tks = tks->next ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, lexicalError) ;
        TS_ASSERT_EQUALS (tks->lexeme, "&");
        tks = tks->next ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, intConst) ;
        TS_ASSERT_EQUALS (tks->lexeme, "1");
        tks = tks->next ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
        TS_ASSERT (tks->next == NULL) ;
    }


    // A test for scanning numbers and a variable.
    void test_terminal_scan_nums_vars ( ) {
        Token *tks = s->scan (" 123 x 12.34 ") ;
        TS_ASSERT (tks != NULL) ;
        tokenType ts[] = { intConst, variableName, floatConst, endOfFile } ;
        TS_ASSERT ( sameTerminals ( tks, 4, ts ) ) ;
    }



    /* This test checks that the scanner returns a list of tokens with
       the correct terminal fields.  It doesn't check that the lexeme
       are correct.
     */
    
    void test_terminal_scan_bad_syntax_good_tokens ( ) {
        const char *filename = "../samples/bad_syntax_good_tokens.dsl" ;
        char *text =  readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;
        Token *tks = s->scan ( text ) ;
        TS_ASSERT (tks != NULL) ;
        tokenType ts[] = { 
            intConst, intConst, intConst, intConst, 

            stringConst, stringConst, stringConst,

            floatConst, floatConst, floatConst,

                   matrixKwd,



            semiColon, comma, colon,
            leftCurly, leftParen, rightCurly, rightParen,

            plusSign, star, dash, forwardSlash, 

            equalsEquals, lessThanEqual, 
            greaterThanEqual, notEquals, 
            assign,

            variableName, variableName, variableName, variableName, 
            variableName, variableName, variableName,


            intKwd, floatKwd,  stringKwd,  whileKwd,

            endOfFile
       } ;
        int count = 39; 
        TS_ASSERT ( sameTerminals ( tks, count, ts ) ) ;
    }

    void test_terminal_scan_sample_forestLoss ( ) {
        scanFileNoLexicalErrors ("../samples/forest_loss_v2.dsl") ;
    }

      

} ;
