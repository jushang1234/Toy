#include <cxxtest/TestSuite.h>

#include "readInput.h"
#include "scanner.h"
#include "Parser.h"

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

		void tearDown() {
			delete s;
			delete p;
		}

	// your tests here!



} ;
