#include <cxxtest/TestSuite.h>

#include "readInput.h"
#include "scanner.h"
#include "ExtToken.h"
#include "Parser.h"
#include "ParseResult.h"
// One of the above has likely included the .h file with your
// definition of the Node class.  It is not named here so that
// you can name your files as you see fit.

using namespace std ;

class AstTestSuite : public CxxTest::TestSuite 
{
public:
    int DEBUG ;

    Scanner *s ;
    Parser *p ;
    void setUp ( ) {
        s = new Scanner() ;
        p = new Parser() ;
        DEBUG = 1 ;
    }

	void tearDown() {
		delete s;
		delete p;
	}

    void unparse_tests ( const char *filename ) {

        string file = filename ;

        // 1. Test that the file can be parsed.
        char *text =  readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;

        if (DEBUG) {
        TS_TRACE ("==================================================") ;
        TS_TRACE ("==================================================") ;
        TS_TRACE ( (string) "\n" + text + "\n" ) ;
        }

        ParseResult pr1 = p->parse ( text ) ;
        TSM_ASSERT ( file + " failed to parse.", pr1.ok ) ;

        // 2. Verify that the ast field is not null
        TSM_ASSERT ( file + " failed to generate an AST.", pr1.ast != NULL );

        // 3. Verify that the "unparsing" is non-empty.
        string up1 = pr1.ast->unparse() ;
        TSM_ASSERT ( file + " failed to generate non-empty unparsing.", 
                     up1.length() > 0 ) ;

        if (DEBUG) {
        TS_TRACE ("= pr1 ============================================") ;
        TS_TRACE ( (string) "\n" + up1 + "\n" ) ;
        }

        // 4. Verify that the un-parsed string can be parsed.
        ParseResult pr2 = p->parse ( up1.c_str() ) ;
        TSM_ASSERT ( file + " failed to parse the first un-parsing.", 
                     pr2.ok ) ;

        // 5. Verify that the ast field is not null after first unparsing
        TSM_ASSERT ( file + " first unparsing failed to generate an AST.", 
                     pr2.ast != NULL );

        // 6. Verify that this second unparsing can be parsed.
        string up2 = pr2.ast->unparse() ;

        if (DEBUG) {
        TS_TRACE ("= up2 ============================================") ;
        TS_TRACE ( (string) "\n" + up2 + "\n" ) ;
        }

        ParseResult pr3 = p->parse ( up2.c_str() ) ;
        TSM_ASSERT ( file + " failed to pares the second un-parsing.",
                     pr3.ok ) ;

        if (DEBUG) {
        TS_TRACE ("= pr3 ==============================================") ;
        TS_TRACE ( (string) "\n" + pr3.ast->unparse() + "\n" ) ;
        }

        string up3 = pr3.ast->unparse() ;

        // 7. Verify that the first and second unparsings are the same.
        TSM_ASSERT_EQUALS ( file + " unparse-1 != unparse-2.", up1, up2 ) ;
        // 8. Verifty that the second and third unparsings are the same.
        TSM_ASSERT_EQUALS ( file + " unparse-2 != unparse-3.", up2, up3 ) ;
    }

    void test_ast_simple_1 ( ) {
        unparse_tests ("../samples/simple_1.kix") ;
    }
    void test_ast_simple_2 ( ) {
        unparse_tests ("../samples/simple_2.kix") ;
    }
    void test_ast_simple_3 ( ) {
        unparse_tests ("../samples/simple_3.kix") ;
    }
    void test_ast_squares ( ) {
        unparse_tests ("../samples/squares.kix") ;
    }
    void test_ast_evenSquares ( ) {
        unparse_tests ("../samples/evenSquares.kix") ;
    }
    void test_ast_matVecMul ( ) {
        unparse_tests ("../samples/matVecMul.kix") ;
    }
    void test_ast_matVecMulLiterals ( ) {
        unparse_tests ("../samples/matVecMulLiterals.kix") ;
    }

} ;
