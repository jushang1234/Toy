#include <cxxtest/TestSuite.h>
#include "readInput.h"
#include "scanner.h"

using namespace std ;

class ScannerTestSuite : public CxxTest::TestSuite 
{
	public:
		Scanner *s ;
		//scanner created before each test.
		
		void setUp() {
			s = new Scanner() ;
		}

		void tearDown()	{
			delete s;
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
		bool sameTerminals (Token *tks, int numTerms, terminal_t *ts) {
			Token *currentToken = tks ;
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

		// The "endOfFile" token is always the last one in the list of tokens.
		void test_scan_empty ( ) {
			Token *tks = s->scan ("  ") ;
			TS_ASSERT (tks != NULL) ;
			TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
			TS_ASSERT (tks->next == NULL) ;
		}

		void test_scan_empty_comment ( ) {
			Token *tks = s->scan (" /* a comment */ ") ;
			TS_ASSERT (tks != NULL) ;
			TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
			TS_ASSERT (tks->next == NULL) ;
		}


		// When a lexical error occurs, the scanner creates a token with a 
		// single-character lexeme and lexicalError as the terminal.
		void test_scan_lexicalErrors ( ) {
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
		void test_scan_nums_vars ( ) {
			Token *tks = s->scan (" 123 x 12.34 ") ;
			TS_ASSERT (tks != NULL) ;
			terminal_t ts[] = { intConst, variableName, floatConst, endOfFile } ;
			TS_ASSERT ( sameTerminals ( tks, 4, ts ) ) ;
		}



		/* This test checks that the scanner returns a list of tokens with
		   the correct terminal fields.  It doesn't check that the lexeme
		   are correct.
		 */
		void test_scan_bad_syntax_good_tokens ( ) {
			const char *filename = "../samples/bad_syntax_good_tokens.kix" ;
			char *text =  readInputFromFile ( filename )  ;
			TS_ASSERT ( text ) ;
			Token *tks = s->scan ( text ) ;
			TS_ASSERT (tks != NULL) ;
			terminal_t ts[] = { 
				intConst, intConst, intConst, intConst, 

				stringConst, stringConst,

				leftSquare, floatConst, comma, floatConst, comma,
				floatConst, rightSquare,

				semiColon, comma, leftParen, leftSquare,
				rightParen, rightSquare ,

				equalsSign, plusSign, star, dash, forwardSlash, percent,
				rightAngle, leftAngle, colon, backSlash,

				colonColon, arrow, equalsEquals, lessThanEquals, 
				greaterThanEquals, notEquals, plusPlus, dotDot,

				variableName, variableName, variableName, variableName, 
				variableName, variableName, variableName,

				variableName, // was mainKwd, 
				letKwd, inKwd, endKwd, ifKwd, thenKwd, elseKwd, 
				printKwd, readKwd, writeKwd, 
				integerKwd, floatKwd, booleanKwd, stringKwd, 
				trueKwd, falseKwd, headKwd, tailKwd, nullKwd, 
				mapKwd, filterKwd, foldKwd,

				tupleOp, tupleOp, tupleOp, tupleOp, tupleOp, 

				endOfFile
			} ;
			int count = 100 ;
			TS_ASSERT ( sameTerminals ( tks, count, ts ) ) ;
		}

		void test_scan_simple_1 ( ) {
			scanFileNoLexicalErrors ("../samples/simple_1.kix") ;
		}
		void test_scan_simple_2 ( ) {
			scanFileNoLexicalErrors ("../samples/simple_2.kix") ;
		}
		void test_scan_simple_3 ( ) {
			scanFileNoLexicalErrors ("../samples/simple_3.kix") ;
		}
		void test_scan_squares ( ) {
			scanFileNoLexicalErrors ("../samples/squares.kix") ;
		}
		void test_scan_evenSquares ( ) {
			scanFileNoLexicalErrors ("../samples/evenSquares.kix") ;
		}
		void test_scan_matVecMulLiterals ( ) {
			scanFileNoLexicalErrors ("../samples/matVecMulLiterals.kix") ;
		}
		void test_scan_matVecMul ( ) {
			scanFileNoLexicalErrors ("../samples/matVecMul.kix") ;
		}


} ;





