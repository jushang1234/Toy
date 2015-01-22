#include <cxxtest/TestSuite.h>

#include "readInput.h"
#include "scanner.h"

using namespace std;

class ScannerTestSuite : public CxxTest::TestSuite {
public:
    /* A Scanner object is created in the test_setup_code method.
       This is a test, beginning with "test_", so that it is executed
       by the testing framework.  The scanner is used in some tests of
       the method "scan".
     */
    Scanner *s;

    void test_setup_code() {
        s = new Scanner();
        TS_ASSERT(s);
    }
    // void test_consume_white_space_and_comments(){
    //     TS_ASSERT_EQUALS(s->consume_white_space_and_comments("/* */ name") == 6);
    //     TS_ASSERT_EQUALS(s->consume_white_space_and_comments("    \n  //   \n /* */ name") == 20);
    // }
    // void test_consume_white_space_and_comments_empty(){
    //     TS_ASSERT_EQUALS(s->consume_white_space_and_comments("   ") == 3);
    // }


    // Tests for components and functions used by "scan"
    // --------------------------------------------------

    /* You will need to write several tests to adequately test the
       functions that are called from "scan".  Once you are confident
       that the components used by "scan" work properly, then you can
       run tests on the "scan" method itself.
     */

    // Here you should place tests for these other functions.
    // See i1_assessment_tests.h and your CxxTest lab for examples


    /* Remember, you have to write a test for each of the terminal
     * types in the language - each terminal is specified in the 
     * tokenEnumType in scanner.h
     */

    //we need, then, one for main words, one for ints, one for floats, one for StringContstants (whatever that is), one for tuples, one for variable names, one for punctuations, one for eof, one for lexerror
   

    void test_make_token_letKwd(){
        Token *tks = s->scan("  let ");
        // cout << "we tests: " << tks->lexeme<< endl;
        TS_ASSERT_EQUALS(tks->terminal, letKwd);
    }
    void test_make_token_inKwd() {
        Token *tks = s->scan("in ");
                // cout << "we tests: " << inKwd<< endl;

        TS_ASSERT_EQUALS(tks->terminal, inKwd);
    }

    void test_make_token_endKwd() {
        Token *tks = s->scan("end ");
                // cout << "we tests: " << endKwd<< endl;

        TS_ASSERT_EQUALS(tks->terminal, endKwd);
    }

    void test_make_token_ifKwd() {
        Token *tks = s->scan("if ");

        TS_ASSERT_EQUALS(tks->terminal, ifKwd);
    }

    void test_make_token_thenKwd() {
        Token *tks = s->scan("then ");

        TS_ASSERT_EQUALS(tks->terminal, thenKwd);
    }

    void test_make_token_elseKwd() {
        Token *tks = s->scan("else ");
        TS_ASSERT_EQUALS(tks->terminal, elseKwd);
    }

    void test_make_token_printKwd() {
        Token *tks = s->scan("print ");
        TS_ASSERT_EQUALS(tks->terminal, printKwd);
    }

    void test_make_token_readKwd() {
        Token *tks = s->scan("read ");
        TS_ASSERT_EQUALS(tks->terminal, readKwd);
    }

    void test_make_token_writeKwd() {
        Token *tks = s->scan("write ");
        TS_ASSERT_EQUALS(tks->terminal, writeKwd);
    }

    void test_make_token_integerKwd() {
        Token *tks = s->scan("Integer ");
        TS_ASSERT_EQUALS(tks->terminal, integerKwd);
    }

    void test_make_token_floatKwd() {
        Token *tks = s->scan("Float ");
        TS_ASSERT_EQUALS(tks->terminal, floatKwd);
    }

    void test_make_token_booleanKwd() {
        Token *tks = s->scan("Boolean ");
        TS_ASSERT_EQUALS(tks->terminal, booleanKwd);
    }

    void test_make_token_stringKwd() {
        Token *tks = s->scan("String ");
        TS_ASSERT_EQUALS(tks->terminal, stringKwd);
    }

    void test_make_token_headKwd() {
        Token *tks = s->scan("head ");
        TS_ASSERT_EQUALS(tks->terminal, headKwd);
    }

    void test_make_token_tailKwd() {
        Token *tks = s->scan("tail ");
        TS_ASSERT_EQUALS(tks->terminal, tailKwd);
    }

    void test_make_token_nullKwd() {
        Token *tks = s->scan("null ");
        TS_ASSERT_EQUALS(tks->terminal, nullKwd);
    }

    void test_make_token_mapKwd() {
        Token *tks = s->scan("map ");
        TS_ASSERT_EQUALS(tks->terminal, mapKwd);
    }

    void test_make_token_filterKwd() {
        Token *tks = s->scan("filter ");
        TS_ASSERT_EQUALS(tks->terminal, filterKwd);
    }

    void test_make_token_foldKwd() {
        Token *tks = s->scan("fold ");
        TS_ASSERT_EQUALS(tks->terminal, foldKwd);
    }

    void test_make_token_zipKwd() {
        Token *tks = s->scan("zip ");
        TS_ASSERT_EQUALS(tks->terminal, zipKwd);
    }

    void test_make_token_trueKwd() {
        Token *tks = s->scan("true ");

        TS_ASSERT_EQUALS(tks->terminal, trueKwd);
    }

    void test_make_token_falseKwd() {
        Token *tks = s->scan("false ");

        TS_ASSERT_EQUALS(tks->terminal, falseKwd);
    }

    void test_make_token_intConst() {
        Token *tks = s->scan("123 ");

        TS_ASSERT_EQUALS(tks->terminal, intConst);
    }

    void test_make_token_floatConst() {
        Token *tks = s->scan("10.1 ");

        TS_ASSERT_EQUALS(tks->terminal, floatConst);
    }

    void test_make_token_floatConst_initial_dot() {
        Token *tks = s->scan("3.123 ");

        TS_ASSERT_EQUALS(tks->terminal, floatConst);
    }

    void test_make_token_stringConst() {
        Token *tks = s->scan("\"trying to\"");

        TS_ASSERT_EQUALS(tks->terminal, stringConst);
    }
   //////////////////////////////////////////////////////////
    void test_make_token_variableName_1() {
        Token *tks = s->scan("fooBar ");
        TS_ASSERT_EQUALS(tks->terminal, variableName);
    }

    void test_make_token_variableName_2() {
        Token *tks = s->scan("fbarvar ");

        TS_ASSERT_EQUALS(tks->terminal, variableName);
    }

    void test_make_token_leftParen() {
        Token *tks = s->scan("  ( ");

        TS_ASSERT_EQUALS(tks->terminal, leftParen);
    }

    void test_make_token_rightParen() {
        Token *tks = s->scan(") ");

        TS_ASSERT_EQUALS(tks->terminal, rightParen);
    }

    void test_make_token_leftSquare() {
        Token *tks = s->scan("[ ");

        TS_ASSERT_EQUALS(tks->terminal, leftSquare);
    }

    void test_make_token_rightSquare() {
        Token *tks = s->scan("  ]");

        TS_ASSERT_EQUALS(tks->terminal, rightSquare);
    }

    void test_make_token_leftAngle() {
        Token *tks = s->scan("< ");

        TS_ASSERT_EQUALS(tks->terminal, leftAngle);
    }

    void test_make_token_rightAngle() {
        Token *tks = s->scan("> ");

        TS_ASSERT_EQUALS(tks->terminal, rightAngle);
    }

    void test_make_token_colon() {
        Token *tks = s->scan(": ");

        TS_ASSERT_EQUALS(tks->terminal, colon);
    }
        void test_make_token_coloncolon() {
        Token *tks = s->scan(":: ");

        TS_ASSERT_EQUALS(tks->terminal, colonColon);
    }

    void test_make_token_comma() {
        Token *tks = s->scan(", ");

        TS_ASSERT_EQUALS(tks->terminal, comma);
    }

    void test_make_token_semiColon() {
        Token *tks = s->scan("; ");

        TS_ASSERT_EQUALS(tks->terminal, semiColon);
    }

    void test_make_token_plusSign() {
        Token *tks = s->scan("+ ");

        TS_ASSERT_EQUALS(tks->terminal, plusSign);
    }

    void test_make_token_star() {
        Token *tks = s->scan("* ");

        TS_ASSERT_EQUALS(tks->terminal, star);
    }

    void test_make_token_dash() {
        Token *tks = s->scan("- ");

        TS_ASSERT_EQUALS(tks->terminal, dash);
    }

    void test_make_token_forwardSlash() {
        Token *tks = s->scan("/ ");

        TS_ASSERT_EQUALS(tks->terminal, forwardSlash);
    }

    void test_make_token_equalsEquals() {
        Token *tks = s->scan("== ");

        TS_ASSERT_EQUALS(tks->terminal, equalsEquals);
    }

    void test_make_token_lessThanEquals() {
        Token *tks = s->scan("<= ");

        TS_ASSERT_EQUALS(tks->terminal, lessThanEquals);
    }

    void test_make_token_greaterThanEquals() {
        Token *tks = s->scan(">= ");

        TS_ASSERT_EQUALS(tks->terminal, greaterThanEquals);
    }

    void test_make_token_notEquals() {
        Token *tks = s->scan("!= ");
        TS_ASSERT_EQUALS(tks->terminal, notEquals);
    }

    void test_make_token_arrow() {
        Token *tks = s->scan("->") ;
        TS_ASSERT_EQUALS(tks->terminal, arrow);
    }

    void test_make_token_backSlash() {
        Token *tks = s->scan("\\");
        TS_ASSERT_EQUALS(tks->terminal, backSlash);
    }

    void test_make_token_percent() {
        Token *tks = s->scan("%");
        TS_ASSERT_EQUALS(tks->terminal, percent);
    }

        void test_make_token_dotDot() {
        Token *tks = s->scan("..");
        TS_ASSERT_EQUALS(tks->terminal, dotDot);
    }

        void test_make_token_plusPlus() {
        Token *tks = s->scan("++");
        TS_ASSERT_EQUALS(tks->terminal, plusPlus);
    }
        void test_make_token_tupleop(){
        Token *tks = s->scan("#12");
        TS_ASSERT_EQUALS(tks->terminal, tupleOp);
        }
        void test_make_token_endoffile(){
        Token *tks = s->scan("\0");
        TS_ASSERT_EQUALS(tks->terminal, endOfFile);
        }
                 
};





