#include <cxxtest/TestSuite.h>

#include "readInput.h"
#include "scanner.h"
#include "ExtToken.h"
#include "Parser.h"
#include "ParseResult.h"

using namespace std ;

class ParserTestSuite : public CxxTest::TestSuite 
{
public:
    Scanner *s ;
    Parser *p ;
    void setUp ( ) {
        s = new Scanner() ;
        p = new Parser() ;
    }

	void tearDown(){
		delete s;
		delete p;
	}

    /* This function checks that the terminal fields in the list of
       tokens matches a list of terminals.
    */
    bool sameTerminalsExt (ExtToken *tks, int numTerms, terminal_t *ts) {
        ExtToken *currentToken = tks ;
        int termIndex = 0 ;
        while (currentToken != NULL && termIndex < numTerms ) {
            if (currentToken->terminal != ts[termIndex]) {
                return false ;
            }
            else {
                ++ termIndex ;
                currentToken = currentToken->next ;
            }
        }
        return true ;
    }

    void test_scan_nums_vars ( ) {
        Token *tks = s->scan ("a :: Integer = 4 ; ") ;
        TS_ASSERT (tks != NULL) ;
        ExtToken *etks = extendTokenList (p, tks ) ;
        TS_ASSERT (etks != NULL) ;
        terminal_t ts[] = { variableName, colonColon, integerKwd, equalsSign,
                            intConst, semiColon, endOfFile } ;
        TS_ASSERT ( sameTerminalsExt ( etks, 8, ts ) ) ;
    }


    void test_parse_dcl ( ) {
        ParseResult pr = p->parse ("a :: Integer = 4 + 1 * 8 - 2 % 4 ; ") ;
        TSM_ASSERT ( pr.errors, pr.ok ) ;
    }

    void test_parse_dcl_fail ( ) {
        ParseResult pr = p->parse ("a :: ; Integer = 4 ;") ;
        TSM_ASSERT ( pr.errors, ! pr.ok ) ;
    }

    void test_parse_dcls ( ) {
        ParseResult pr = p->parse ("a :: Integer = 4 ; b :: Integer = 5 ;") ;
        TSM_ASSERT ( pr.errors, pr.ok ) ;
    }

    void test_parse_dcls_fail ( ) {
        ParseResult pr = p->parse ("a :: Integer = 4 ; ; b :: Integer = 5 ;") ;
        TSM_ASSERT ( pr.errors,  ! pr.ok ) ;
    }

    void parseFileNoErrors ( const char* filename ) {
        char *text =  readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;
        ParseResult pr = p->parse ( text ) ;
        string msg (filename) ;
        msg += "\n" + pr.errors + "\n" + pr.pp ;
        TSM_ASSERT ( msg , pr.ok );
    }

    void test_parse_simple_1 ( ) {
        parseFileNoErrors ("../samples/simple_1.kix") ;
    }
    void test_parse_simple_2 ( ) {
        parseFileNoErrors ("../samples/simple_2.kix") ;
    }
    void test_parse_simple_3 ( ) {
        parseFileNoErrors ("../samples/simple_3.kix") ;
    }
    void test_parse_squares ( ) {
        parseFileNoErrors ("../samples/squares.kix") ;
    }
    void test_parse_evenSquares ( ) {
        parseFileNoErrors ("../samples/evenSquares.kix") ;
    }
    void test_parse_matVecMul ( ) {
        parseFileNoErrors ("../samples/matVecMul.kix") ;
    }
    void test_parse_matVecMulLiterals ( ) {
        parseFileNoErrors ("../samples/matVecMulLiterals.kix") ;
    }


} ;
