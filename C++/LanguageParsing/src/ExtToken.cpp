/* ExtToken: an extension of the Token class with new methods for parsing 
   (led, nud, lbp) and describing the token.

   Author: Eric Van Wyk

   Last modified: 3/4/2014.
*/


#include "ExtToken.h"
#include "Parser.h"

#include <string>

using namespace std ;

ExtToken *extendToken (Parser *p, Token *tokens) {
    switch ( tokens->terminal ) {

    case letKwd: return new LetKwdToken(p,tokens) ;
    case inKwd: return new InKwdToken(p,tokens) ;
    case endKwd: return new EndKwdToken(p,tokens) ;

    case ifKwd: return new IfKwdToken(p,tokens) ;
    case thenKwd: return new ThenKwdToken(p,tokens) ;
    case elseKwd: return new ElseKwdToken(p,tokens) ;

    case printKwd: return new PrintKwdToken(p,tokens) ;
    case writeKwd: return new WriteKwdToken(p,tokens) ;
    case readKwd: return new ReadKwdToken(p,tokens) ;

    case integerKwd: return new IntegerKwdToken(p,tokens) ;
    case floatKwd: return new FloatKwdToken(p,tokens) ;
    case booleanKwd: return new BooleanKwdToken(p,tokens) ;
    case stringKwd: return new StringKwdToken(p,tokens) ;

    case trueKwd: return new TrueKwdToken(p,tokens) ;
    case falseKwd: return new FalseKwdToken(p,tokens) ;

    case headKwd: return new HeadKwdToken(p,tokens) ;
    case tailKwd: return new TailKwdToken(p,tokens) ;
    case nullKwd: return new NullKwdToken(p,tokens) ;
    case mapKwd: return new MapKwdToken(p,tokens) ;
    case filterKwd: return new FilterKwdToken(p,tokens) ;
    case foldKwd: return new FoldKwdToken(p,tokens) ;
    case zipKwd: return new ZipKwdToken(p,tokens) ;

    case intConst: return new IntConstToken(p,tokens) ;
    case floatConst: return new FloatConstToken(p,tokens) ;
    case stringConst: return new StringConstToken(p,tokens) ;

    case variableName: return new VariableNameToken(p,tokens) ;

    case leftParen: return new LeftParenToken(p,tokens) ;
    case rightParen: return new ExtToken(p,tokens,")") ;
    case leftSquare: return new LeftSquareToken(p,tokens) ;
    case rightSquare: return new ExtToken(p,tokens,"]") ;
    case leftAngle: return new LeftAngleToken(p,tokens) ;
    case rightAngle: return new RightAngleToken(p,tokens) ;

    case colon: return new ColonToken(p,tokens) ;
    case colonColon: return new ColonColonToken(p,tokens) ;
    case comma: return new ExtToken(p,tokens,",") ;
    case semiColon: return new SemiColonToken(p,tokens) ;
    case equalsSign: return new EqualsSignToken(p,tokens) ;

    case plusSign: return new PlusSignToken(p,tokens) ;
    case dash: return new DashToken(p,tokens) ;
    case star: return new StarToken(p,tokens) ;
    case forwardSlash: return new ForwardSlashToken(p,tokens) ;
    case percent: return new PercentToken(p,tokens) ;

    case backSlash: return new BackSlashToken(p,tokens) ;
    case arrow: return new ExtToken(p,tokens,"->") ;

    case lessThanEquals: 
    case greaterThanEquals: 
    case equalsEquals: 
    case notEquals: 
        return new RelationalOpToken(p, tokens, tokens->lexeme) ;

    case plusPlus: return new PlusPlusToken(p,tokens) ;
    case dotDot: return new DotDotToken(p,tokens) ;
    case tupleOp: return new TupleOpToken(p,tokens) ;

    case lexicalError: return new ExtToken(p,tokens,"lexical error") ;
    case endOfFile: return new EndOfFileToken(p,tokens) ;

    default: 
        string msg = (string) "Unspecified terminal in extend." ;
        throw ( p->makeErrorMsg ( msg.c_str() ) ) ;
    }
}

ExtToken *extendTokenList (Parser *p, Token *tokens) {
    ExtToken *extTokens = NULL ;
    ExtToken *prevExtToken = NULL ;

    while (tokens != NULL) {
        ExtToken *currExtToken = extendToken(p, tokens) ;
 
        // Add new ExtToken et to the list
        if (extTokens != NULL) {
            prevExtToken->next = currExtToken ;
        }
        else {
            extTokens = currExtToken ;
        }
        prevExtToken = currExtToken ;

        tokens = tokens->next ;
    }

    return extTokens ;
}
