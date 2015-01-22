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

   Last modified: March 4, 2014.
*/

#include "Parser.h"
#include "scanner.h"
#include "ExtToken.h"
#include "AST.h"

#include <assert.h>

using namespace std ;

Parser::Parser ( ) { } 

ParseResult Parser::parse (const char *text) {
    assert (text != NULL) ;

    ParseResult pr ;
    try {
        Scanner *s = new Scanner() ;
        tokens = extendTokenList ( this, s->scan (text) ) ;

        assert (tokens != NULL) ;
        currToken = tokens ;
        pr = parseProgram( ) ;
    }
    catch (string errMsg) {
        pr.ok = false ;
        pr.errors = errMsg ;
        pr.ast = NULL ;
    }
    return pr ;
}

/* 
 * parse methods for non-terminal symbols
 * --------------------------------------
 */

// Program DONE
ParseResult Parser::parseProgram () {
    ParseResult pr ;
	pr.ast = NULL;
    // Program ::= DeclList
    parseDeclList() ;

    // verify that ast field is a DeclList pointer

    match(endOfFile) ;

    return pr ;
}

// DeclList DONE
ParseResult Parser::parseDeclList () {
    ParseResult pr ;
    // DeclList ::= Decl
    parseDecl() ;

    // verify that ast field is a Declt pointer

    if ( nextIs(variableName) ) {
        // DeclList ::= Decl DeclList
        parseDeclList() ;

        // verify that ast field is a DeclList pointer
    }
    else {
		//emptyList
    }

    return pr ;
}

// Decl DONE
ParseResult Parser::parseDecl () {
    ParseResult pr ;
    // Decl ::= variableName '::' TypeExpr '=' Expr ';'

    match(variableName) ;

    match(colonColon) ;

    parseTypeExpr() ;

    // verify that ast field is a TypeExpr pointer

    match(equalsSign) ;
    parseExpr(0) ;


    match(semiColon) ;

    return pr ;
}

// TypeExpr DONE
ParseResult Parser::parseTypeExpr () {
    ParseResult pr ;
    if ( attemptMatch(integerKwd) ) { 
        // TypeExpr ::= 'Integer' DONE
    }

    else if ( attemptMatch(floatKwd) ) {
        // TypeExpr ::= 'Float' DONE
    }

    else if ( attemptMatch(stringKwd) ) {
        // TypeExpr ::= 'String' DONE
    }

    else if ( attemptMatch(booleanKwd) ) {
        // TypeExpr ::= 'Boolean' DONE
    }

    else if ( attemptMatch(leftSquare) ) {
        // TypeExpr ::= '[' TypeExpr ']' DONE

        parseTypeExpr() ;

        match(rightSquare) ;

    }

    else if ( attemptMatch(leftParen) ) {
        if ( nextIs(variableName) ) {
            // TypeExpr ::= '(' TypeBindingList ')' '->' TypeExpr DONE

            parseTypeBindingList() ;


            match(rightParen) ;
            match(arrow) ;

            parseTypeExpr() ;

        } else {
            // TypeExpr ::= '(' TypeExprList ')'  DONE

            parseTypeExprList() ;
            match(rightParen) ;

        }
    }
    else {
        throw ( makeErrorMsg ( currToken->terminal ) ) ;
    }

    return pr ;
}

// TypeExprList DONE
ParseResult Parser::parseTypeExprList () {
    ParseResult pr ;
    // TypeExprList ::= TypeExpr

    parseTypeExpr() ;

    if ( attemptMatch(comma) ) {
        // TypeExprList ::= TypeExpr ',' TypeExprList

        parseTypeExprList () ;

    }
    else {
       //no TypeExprs follow the one matched previously in this function. 

    }    
    
    return pr ;
}

// TypeBindingList DONE
ParseResult Parser::parseTypeBindingList () {
    ParseResult pr ;
    // TypeBindingList ::= TypeBinding

    parseTypeBinding () ;


    if ( attemptMatch(comma) ) {
        // TypeBindingList ::= TypeBinding ',' TypeBindingList

        parseTypeBindingList () ;

    }
    else {
    }
    
    return pr ;
}

// TypeBinding DONE
ParseResult Parser::parseTypeBinding () {
    ParseResult pr ;
    // TypeBinding ::= variableName '::' TypeExpr
    match(variableName) ;
    match(colonColon) ;

    parseTypeExpr () ;

    return pr ;
}

// OptionalExprList DONE
ParseResult Parser::parseOptionalExprList () {
    ParseResult pr ;
    if ( nextIs(rightSquare) || nextIs(rightParen) ) {
        // OptionalExprList ::= <<empty>>
    } else {
        // OptionalExprList ::= ExprList
        parseExprList() ;

    }
    return pr ;
}

// ExprList  DONE
ParseResult Parser::parseExprList () {
    ParseResult pr ;
    // ExprList ::= Expr

    parseExpr(0) ;

    if ( attemptMatch(comma) ) {
        // ExprList ::= Expr ',' ExprList
        parseExprList() ;

    }
    else {
		//empty listExprs
    }
    
    return pr ;
}

// Expr
ParseResult Parser::parseExpr (int rbp) {
    /* Examine current token, without consuming it, to call its
       associated parse methods.  The ExtToken objects have 'nud' and
       'led' methods that are dispatchers that call the appropriate
       parse methods.*/
    ParseResult left = currToken->nud() ;
   
    while (rbp < currToken->lbp() ) {
        left = currToken->led(left) ;
    }


    return left ;
}

/*
 * parse methods for productions
 * -----------------------------
 */

// Expr ::= variableName DONE
ParseResult Parser::parseVariableName ( ) {
    ParseResult pr ;
    match ( variableName ) ;
    return pr ;
}

// Expr ::= intConst DONE
ParseResult Parser::parseIntConst ( ) {
    ParseResult pr ;
    match ( intConst ) ;
    return pr ;
}

// Expr ::= floatConst DONE
ParseResult Parser::parseFloatConst ( ) {
    ParseResult pr ;
    match ( floatConst ) ;
    return pr ;
}

// Expr ::= stringConst DONE
ParseResult Parser::parseStringConst ( ) {
    ParseResult pr ;
    match ( stringConst ) ;
    return pr ;
}

// Expr ::= trueKwd DONE
ParseResult Parser::parseTrueKwd ( ) {
    ParseResult pr ;
    match ( trueKwd ) ;
    return pr ;
}

// Expr ::= falseKwd DONE
ParseResult Parser::parseFalseKwd ( ) {
    ParseResult pr ;
    match ( falseKwd ) ;
    return pr ;
}


ParseResult Parser::parseNestedOrTupleExpr ( ) {
    ParseResult pr ;
    match ( leftParen ) ;


    // Expr ::= '(' Expr ')'  DONE
    parseExpr(0) ; 

    if ( attemptMatch(comma) ) {
        // Expr ::= '(' Expr ',' ExprList ')'  DONE
        parseExprList() ;
    }
    else {
		//no ExprList to parse 
    }

    match(rightParen) ;

    return pr ;
}

// Expr ::= '[' ExprList ']'  Original documentation
// Expr ::= '[' OptionalExprList ']'  Original implementation

// Expr ::= '[' OptionalExprList ']'  before adding TypeExpr to literals
//       |  '[' Expr '..' Expr ']'     

// Expr ::= '[' '<' TypeExpr '>' OptionalExprList ']'  after TypeExpr
//       |  '[' Expr '..' Expr ']'     

ParseResult Parser::parseBraketListExpr ( ) {
    ParseResult pr ;
    match ( leftSquare ) ;

    if ( attemptMatch(leftAngle) ) {

        parseTypeExpr() ;

        match(rightAngle) ;

        parseOptionalExprList() ; 

    }
    else {
        parseExpr(0); 
        match(dotDot) ; 
        parseExpr(0); 

    }
    match(rightSquare) ;
    return pr ;
}

// Expr ::= variableName '(' OptionnalExprList ')'  DONE
ParseResult Parser::parseFunctionCall ( ParseResult left ) {
    // parser has already matched variableName on left.ast


    ParseResult pr ;

    match ( leftParen ) ;

    parseOptionalExprList() ; 

    match(rightParen) ;

    return pr ;
}


// Expr ::= Expr '+' Expr  DONE
ParseResult Parser::parseAddition ( ParseResult left ) {
    // parser has already matched left expression 

    ParseResult pr ;
    match ( plusSign ) ;

    parseExpr( prevToken->lbp() ); 

    return pr ;
}

// Expr ::= Expr '*' Expr  DONE
ParseResult Parser::parseMultiplication ( ParseResult left ) {
    // parser has already matched left expression 

    ParseResult pr ;

    match ( star ) ;

    parseExpr( prevToken->lbp() ); 


    return pr ;
}
// Expr ::= Expr '-' Expr  DONE
ParseResult Parser::parseSubtraction ( ParseResult left ) {
    // parser has already matched left expression 

    ParseResult pr ;

    match ( dash ) ;

    parseExpr( prevToken->lbp() ); 

    return pr ;
}
// Expr ::= Expr '/' Expr  DONE
ParseResult Parser::parseDivision ( ParseResult left ) {
    // parser has already matched left expression 
    ParseResult pr ;

    match ( forwardSlash ) ;

    parseExpr( prevToken->lbp() ); 


    return pr ;
}
// Expr ::= Expr '%' Expr  DONE
ParseResult Parser::parseModulus ( ParseResult left ) {
    // parser has already matched left expression 

    ParseResult pr ;

    match ( percent ) ;

    parseExpr( prevToken->lbp() ); 

    return pr ;
}

// Expr ::= Expr '==' Expr  DONE
// Expr ::= Expr '<' Expr
// Expr ::= Expr '>' Expr
// Expr ::= Expr '>=' Expr
// Expr ::= Expr '<=' Expr
// Expr ::= Expr '!=' Expr
/* Notice that for relational operators we use just one parse
   function.  This shows another possible means for implementing
   expressions, as opposed to the method used for arithmetic
   expressions in which each operation has its own parse method.  It
   will depend on what we do in iteration 3 in building an abstract
   syntax tree to decide which method is better.
 */
ParseResult Parser::parseRelationalExpr ( ParseResult left ) {
    // parser has already matched left expression 


    ParseResult pr ;

    nextToken( ) ;
    // just advance token, since examining it in parseExpr led 
    // to this method being called.
    

    parseExpr( prevToken->lbp() ); 


    return pr ;
}

// Expr ::= Expr ':' Expr  DONE
ParseResult Parser::parseConsExpr ( ParseResult left ) {
    // parser has already matched left expression 

    ParseResult pr ;

    match(colon) ; 

    parseExpr( prevToken->lbp() ); 

    return pr ;
}

// Expr ::= Expr '++' Expr  DONE
ParseResult Parser::parseAppendExpr ( ParseResult left ) {
    // parser has already matched left expression 

    ParseResult pr ;

    match(plusPlus) ; 

    parseExpr( prevToken->lbp() ); 

    return pr ;
}
/*
// Expr ::= '[' Expr '..' Expr ']'  DONE
ParseResult Parser::parseDotDotExpr ( ParseResult left ) {
    // parser has already matched left expression 
    Expr *e1 = dynamic_cast<Expr *>(left.ast) ;
    if ( ! e1 ) throw ( (string) "Bad cast of first Expr in ListRange" ) ;

    ParseResult pr ;

    match(dotDot) ; 

    ParseResult prExpr = parseExpr( prevToken->lbp() ); 

    Expr *e2 = dynamic_cast<Expr *>(prExpr.ast) ;
    if ( ! e2 ) throw ( (string) "Bad cast of second Expr in ListRange" ) ;
//    throw ((string) "IN LIST RANGE");
    pr.ast = new ListRange (e1, e2) ;
    return pr ;
}
*/

// Expr ::= 'let' DeclList 'in' Expr 'end'  DONE
ParseResult Parser::parseLetExpr ( ) {
    ParseResult pr ;
    match(letKwd) ; 

    parseDeclList() ;


    match(inKwd) ; 

    parseExpr(0); 

    match(endKwd) ; 

    return pr ;
}

// Expr ::= 'if' Expr 'then' Expr 'else' Expr  DONE
ParseResult Parser::parseIfThenElse ( ) {
    ParseResult pr ;
    match(ifKwd) ;

    parseExpr(0) ;

    match(thenKwd) ;

    parseExpr(0) ;

    match(elseKwd);

    parseExpr(0) ;

    return pr ;
}

// Expr ::= 'print' '(' Expr ')'  DONE
ParseResult Parser::parsePrintExpr ( ) {
    ParseResult pr ;
    match(printKwd) ;
    match(leftParen) ;

    parseExpr(0) ;

    match(rightParen) ;

    return pr ;
}

// Expr ::= 'write' '(' Expr, Expr ')'  
ParseResult Parser::parseWriteExpr ( ) {
    ParseResult pr ;
    match(writeKwd) ;

    match(leftParen) ;

    parseExpr(0) ;

    match(comma) ;

    parseExpr(0) ;

    match(rightParen) ;

    return pr ;
}

// Expr ::= 'read' '<' TypeExpr '>' '(' Expr ')'
ParseResult Parser::parseReadExpr ( ) {
    ParseResult pr ;
    match(readKwd) ;

    match(leftAngle) ;

    parseTypeExpr() ;

    match(rightAngle) ;
    match(leftParen) ;

    parseExpr(0) ;

    match(rightParen) ;

    return pr ;
}

// Expr ::= 'head' '(' Expr ')'
ParseResult Parser::parseHeadExpr ( ) {
    ParseResult pr ;
    match(headKwd) ;
    match(leftParen) ;

    parseExpr(0) ;

    match(rightParen) ;

    return pr ;
}

// Expr ::= 'tail' '(' Expr ')'
ParseResult Parser::parseTailExpr ( ) {
    ParseResult pr ;
    match(tailKwd) ;
    match(leftParen) ;

    parseExpr(0) ;

    match(rightParen) ;

    return pr ;
}

// Expr ::= 'null' '(' Expr ')'
ParseResult Parser::parseNullExpr ( ) {
    ParseResult pr ;
    match(nullKwd) ;
    match(leftParen) ;

    parseExpr(0) ;

    match(rightParen) ;

    return pr ;
}

// Expr ::= 'map' '(' Expr ',' Expr ')'
ParseResult Parser::parseMapExpr ( ) {
    ParseResult pr ;

    match(mapKwd) ;
    match(leftParen) ;

    parseExpr(0) ;

    match(comma) ;

    parseExpr(0) ;

    match(rightParen) ;

    return pr ;
}

// Expr ::= 'filter' '(' Expr ',' Expr ')'
ParseResult Parser::parseFilterExpr ( ) {
    ParseResult pr ;

    match(filterKwd) ;
    match(leftParen) ;

    parseExpr(0) ;

    match(comma) ;

    parseExpr(0) ;

    match(rightParen) ;

    return pr ;
}

// Expr ::= 'fold' '(' Expr ',' Expr ',' Expr ')'
ParseResult Parser::parseFoldExpr ( ) {
    ParseResult pr ;

    match(foldKwd) ;
    match(leftParen) ;

    parseExpr(0) ;

    match(comma) ;

    parseExpr(0) ;

    match(comma) ;

    parseExpr(0) ;

    match(rightParen) ;

    return pr ;
}

// Expr ::= 'zip' '(' Expr ',' Expr ')'
ParseResult Parser::parseZipExpr ( ) {
    ParseResult pr ;

    match(zipKwd) ;
    match(leftParen) ;

    parseExpr(0) ;

    match(comma) ;

    parseExpr(0) ;

    match(rightParen) ;

    return pr ;
}

// Expr ::= '\' TypeBindingsList 'arrow' Expr
ParseResult Parser::parseLambdaExpr ( ) {
    ParseResult pr ;
    match(backSlash) ;

    parseTypeBindingList() ;

    match(arrow) ;

    parseExpr(0) ;

    return pr ;
}

// Expr ::= tupleOp '(' Expr ')'
ParseResult Parser::parseProjectionExpr () {
    ParseResult pr ;

    match(tupleOp) ;
    ExtToken *op = prevToken ;

    match(leftParen) ;

    parseExpr(0) ;

    match(rightParen) ;

    return pr ;
}


// Helper function used by the parser.

void Parser::match (terminal_t tt) {
    if (! attemptMatch(tt)) {
        throw ( makeErrorMsgExpected ( tt ) ) ;
    }
}

bool Parser::attemptMatch (terminal_t tt) {
    if (currToken->terminal == tt) { 
        nextToken() ;
        return true ;
    }
    return false ;
}

bool Parser::nextIs (terminal_t tt) {
    return currToken->terminal == tt ;
}

void Parser::nextToken () {
    if (currToken->terminal == endOfFile && currToken->next == NULL) {
        prevToken = currToken ;
    } else if (currToken->terminal != endOfFile && currToken->next == NULL) {
        throw ( makeErrorMsg ( "Error: tokens end with endOfFile" ) ) ;
    } else {
        prevToken = currToken ;
        currToken = currToken->next ;
    }
}

string Parser::terminalDescription ( terminal_t terminal ) {
    Token *dummyToken = new Token ("",terminal, NULL) ;
    ExtToken *dummyExtToken = extendToken (this, dummyToken) ;
    string s = dummyExtToken->description() ;
    delete dummyToken ;
    delete dummyExtToken ;
    return s ;
}

string Parser::makeErrorMsgExpected ( terminal_t terminal ) {
    string s = (string) "Expected " + terminalDescription (terminal) +
        " but found " + currToken->description() ;
    return s ;
}

string Parser::makeErrorMsg ( terminal_t terminal ) {
    string s = "Unexpected symbol " + terminalDescription (terminal) ;
    return s ;
}

string Parser::makeErrorMsg ( const char *msg ) {
    return msg ;
}

