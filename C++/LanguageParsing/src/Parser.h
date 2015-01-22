/* A recursive descent parser with operator precedence.

   Author: Eric Van Wyk

   This algorithms is based on the work of Douglas Crockford in "Top
   Down Operator Precedence", a chapter in the book "Beautiful Code".
   Mr. Crockford describes in his chapter how his work comes from that
   of Based on algorithms described Vaughan Pratt in "Top Down Operator
   Precedence", presented at the ACM Symposium on Principles of
   Programming Languages.

   Douglas Crockford's chapter is available at 
    http://javascript.crockford.com/tdop/tdop.html

   Vaughan Pratt's paper is available at 
    http://portal.acm.org/citation.cfm?id=512931

   These are both quite interesting works and worth reading if you
   find the problem of parsing to be an interesting one.

   Last modified: March 4, 2014
*/

#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "ParseResult.h"

#include <string>

class ExtToken ;

class Parser {

public:
    Parser() ;

    ParseResult parse (const char *text) ;

    // methods for parsing non-terminal symbols
    ParseResult parseProgram () ;
    ParseResult parseDeclList () ;
    ParseResult parseDecl () ;
    ParseResult parseTypeExpr () ;
    ParseResult parseTypeExprList () ;
    ParseResult parseTypeBinding () ;
    ParseResult parseTypeBindingList () ;
    ParseResult parseOptionalExprList () ;
    ParseResult parseExprList () ;
    ParseResult parseExpr (int rbp) ;

    // methods for parsing productions for Expr
    ParseResult parseVariableName ( ) ;
    ParseResult parseIntConst ( ) ;
    ParseResult parseFloatConst ( ) ;
    ParseResult parseStringConst ( ) ;
    ParseResult parseTrueKwd ( ) ;
    ParseResult parseFalseKwd ( ) ;

    ParseResult parseNestedOrTupleExpr ( ) ;
    ParseResult parseBraketListExpr ( ) ;
    ParseResult parseFunctionCall ( ParseResult left ) ;

    ParseResult parseAddition ( ParseResult left ) ;
    ParseResult parseSubtraction ( ParseResult left ) ;
    ParseResult parseMultiplication ( ParseResult left ) ;
    ParseResult parseDivision ( ParseResult left ) ;
    ParseResult parseModulus ( ParseResult left ) ;
    ParseResult parseRelationalExpr ( ParseResult left ) ;

    ParseResult parseConsExpr ( ParseResult left ) ;
    ParseResult parseAppendExpr ( ParseResult left ) ;
//  ParseResult parseDotDotExpr ( ParseResult left ) ;

    ParseResult parseLetExpr ( ) ;
    ParseResult parseIfThenElse ( ) ;

    ParseResult parsePrintExpr ( ) ;
    ParseResult parseWriteExpr ( ) ;
    ParseResult parseReadExpr ( ) ;

    ParseResult parseHeadExpr ( ) ;
    ParseResult parseTailExpr ( ) ;
    ParseResult parseNullExpr ( ) ;
    ParseResult parseMapExpr ( ) ;
    ParseResult parseFilterExpr ( ) ;
    ParseResult parseFoldExpr ( ) ;
    ParseResult parseZipExpr ( ) ;

    ParseResult parseLambdaExpr ( ) ;
    ParseResult parseProjectionExpr ( ) ;

    // Helper function used by the parser.
    void match (terminal_t tt) ;
    bool attemptMatch (terminal_t tt) ;
    bool nextIs (terminal_t tt) ;
    void nextToken () ;

    std::string terminalDescription ( terminal_t terminal ) ;
    std::string makeErrorMsg ( terminal_t terminal ) ;
    std::string makeErrorMsgExpected ( terminal_t terminal ) ;
    std::string makeErrorMsg ( const char *msg ) ;

    ExtToken *tokens ;
    ExtToken *currToken ;
    ExtToken *prevToken ;

} ;

#endif /* PARSER_H */
