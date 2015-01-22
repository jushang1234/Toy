/* A small meaningless comment */

#ifndef SCANNER_H
#define SCANNER_H
//it might hurt
#include "regex.h"
#include <regex.h>
#include <string>
#include <vector>


/* This enumerated type is used to keep track of what kind of
   construct was matched. 
*/
enum tokenEnumType { 

    // Keywords
    letKwd, inKwd, endKwd, ifKwd, thenKwd, elseKwd, 
    printKwd, readKwd, writeKwd, 
    
    integerKwd, floatKwd, booleanKwd, stringKwd, 
    trueKwd, falseKwd, 

    headKwd, tailKwd, nullKwd, 
    mapKwd, filterKwd, foldKwd, 
    zipKwd,

    // Constants
    intConst, floatConst, stringConst, 

    // Names
    variableName ,

    // Punctuation
    leftParen, rightParen, 
    leftSquare, rightSquare, leftAngle, rightAngle,
    colon, colonColon, comma, semiColon, 
    equalsSign, 

    plusSign, star, dash, forwardSlash, percent, 
    backSlash, arrow, 

    equalsEquals, 
    lessThanEquals, greaterThanEquals, notEquals, 
    plusPlus, dotDot, 
    tupleOp,

    endOfFile ,
    lexicalError
    
} ;

typedef enum tokenEnumType terminal_t ;

using namespace std;

// Below you need to write your class declarations for Token and Scanner.
class Token {   
    public:
        // int lexLine; //the line the lex is on
        // int lexSlot;     //the slot it starts at, so we can call it again by ourLex[84] or whatever 
        // int lexLength;   //how long it is, so we know when to stop
        //we can also put more stuff in here if we want it. Do we want a string name category in addition oto the string lexeme?    
        
        terminal_t terminal;
        std::string lexeme;     
        Token *next; 
        //Token *prev; //in case we want to make it a doubly linked list?
        Token();
        Token(const char* lexeme, terminal_t t, Token* next);
        // Token(terminal_t terminal, std::string lexeme, int lexLine, int lexSlot, int lexLength)
        
};

class regex {//fuck I hate to do this
    public:
        regex();
        terminal_t terminalname;
        regex_t* expression;
        regex(terminal_t t, regex_t* expression);
        };

class Scanner {
    public:
        regex_t * whiteSpace ;
        regex_t * blockComment ;
        regex_t * lineComment;
        std::vector<regex> regexes; //this contains the shit
        std::vector<regex_t*> skip_regexes;//consumewhitespaceandcomments uses it
        Scanner();
        Token *scan(const char*) ;
        int consumeWhiteSpaceAndComments(regex_t *whiteSpace, 
                                 regex_t *blockComment, 
                                 regex_t *lineComment,
                                 const char *text);//old fashion
        int consumeWhiteSpaceAndComments(const char* text); 



};
#endif /* SCANNER_H */
