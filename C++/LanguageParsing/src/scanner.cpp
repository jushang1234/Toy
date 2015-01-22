/* This program counts words and numbers, except for those not found
   in comments.

   Author(s): Eric Van Wyk
   
   
   OUR NAMES: Zach Buchan, Shawn Ju
   
   
*/

#include "scanner.h"
#include "regex.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>
//#include "getTerminal.h"

using namespace std; 

//I make it looks better...
//basiclly the same shit as we do in WordCount...
//improve by doing it as a loop
//hooray


// it seems like the consume cause the segementation fault, so I changed into the mose fucked up way
// //and it's not working either.

// int Scanner::consumeWhiteSpaceAndComments(regex_t *whiteSpace, 
//                                           regex_t *blockComment, 
//                                           regex_t *lineComment,
//                                           const char *text) {
//     int numMatchedChars = 0 ;
//     int totalNumMatchedChars = 0 ;
//     int stillConsumingWhiteSpace ;

//     do {
//         stillConsumingWhiteSpace = 0 ;  // exit loop if not reset by a match

//         // Try to match white space
//         numMatchedChars = matchRegex (whiteSpace, text) ;
//         totalNumMatchedChars += numMatchedChars ;
//         if (numMatchedChars > 0) {
//             text = text + numMatchedChars ;
//             stillConsumingWhiteSpace = 1 ;
//         }

//         // Try to match block comments
//         numMatchedChars = matchRegex (blockComment, text) ;
//         totalNumMatchedChars += numMatchedChars ;
//         if (numMatchedChars > 0) {
//             text = text + numMatchedChars ;
//             stillConsumingWhiteSpace = 1 ;
//   }
//         // Try to match single-line comments
//         numMatchedChars = matchRegex (lineComment, text) ;
//         totalNumMatchedChars += numMatchedChars ;
//         if (numMatchedChars > 0) {
//             text = text + numMatchedChars ;
//             stillConsumingWhiteSpace = 1 ;
//         }
//     }
//     while ( stillConsumingWhiteSpace ) ;    

//     return totalNumMatchedChars ;

// }

int Scanner::consumeWhiteSpaceAndComments(const char* text) {
    const char* cur_pos = text;
    int ret = 0;
    int cur_matched = 0;
    bool matched = true;
    int regexsizes = skip_regexes.size();
    while (matched) { //continue until no matched left
        matched = false;
        for (unsigned int t = 0; t < regexsizes; ++t) {
            if ( (cur_matched = matchRegex(skip_regexes[t], cur_pos)) ) { //match
                matched = true;
                ret += cur_matched;
                cur_pos += cur_matched;
            }
        }
    }
    return ret;
}


//empty constructors for tokens and scanner
Token::Token(){
    terminal = lexicalError;
    next = NULL;
    lexeme = "";
}


Token::Token(const char* lexeme, terminal_t t, Token* next) {
    this->lexeme = lexeme;
    this->terminal = t;
    this->next = next;
}

regex::regex(terminal_t t, regex_t* expression)
{
  this->terminalname = t;
  this->expression = expression;
}



Scanner::Scanner(){
    this->skip_regexes.push_back(makeRegex("^[\n\t\r ]+"));
    this->skip_regexes.push_back(makeRegex("^/\\*([^\\*]|\\*+[^\\*/])*\\*+/"));
    this->skip_regexes.push_back(makeRegex ( "^//[^\n]*\n"));
    
    // constants
    this->regexes.push_back(regex(stringConst, makeRegex("^\"([^\"]|(\\\\\"))*\"")));
    this->regexes.push_back(regex(intConst, makeRegex ( "^[+-]*([0-9]+)")));    
    this->regexes.push_back(regex(floatConst, makeRegex ( "^[+-]*[0-9]+\\.[0-9]+")));     
    // //keywords
    this->regexes.push_back(regex(tupleOp, makeRegex ("^#[1-9][0-9]*")));
    this->regexes.push_back(regex(inKwd, makeRegex("^in")) ); 
    this->regexes.push_back(regex(endKwd, makeRegex("^end")) );
    this->regexes.push_back(regex(ifKwd, makeRegex("^if") ));
    this->regexes.push_back(regex(thenKwd, makeRegex("^then")) );
    this->regexes.push_back(regex(elseKwd, makeRegex("^else"))) ;
    this->regexes.push_back(regex(printKwd, makeRegex("^print") ));
    this->regexes.push_back(regex(readKwd, makeRegex("^read") ));
    this->regexes.push_back(regex(writeKwd, makeRegex("^write") ));
    this->regexes.push_back(regex(integerKwd, makeRegex("^Integer")) );
    this->regexes.push_back(regex(floatKwd, makeRegex("^Float") ));
    this->regexes.push_back(regex(booleanKwd, makeRegex("^Boolean"))) ;
    this->regexes.push_back(regex(stringKwd, makeRegex("^String") ));
    this->regexes.push_back(regex(trueKwd, makeRegex("^true"))) ;
    this->regexes.push_back(regex(falseKwd, makeRegex("^false")) );
    this->regexes.push_back(regex(headKwd, makeRegex("^head") ));
    this->regexes.push_back(regex(letKwd, makeRegex("^let") ));
    this->regexes.push_back(regex(tailKwd, makeRegex("^tail") ));
    this->regexes.push_back(regex(nullKwd, makeRegex("^null") ));
    this->regexes.push_back(regex(mapKwd, makeRegex("^map") ));
    this->regexes.push_back(regex(filterKwd, makeRegex("^filter"))) ;
    this->regexes.push_back(regex(foldKwd, makeRegex("^fold") ));
    this->regexes.push_back(regex(zipKwd, makeRegex("^zip") ));
	// variable names
    this->regexes.push_back(regex(variableName, makeRegex ("^([a-zA-Z]+)(_*[a-zA-Z]*[0-9]*)*")));
    //punctuations
    this->regexes.push_back(regex(leftParen, makeRegex("^\\(") ));
    this->regexes.push_back(regex(rightParen, makeRegex("^\\)") ));
    this->regexes.push_back(regex(leftSquare, makeRegex("^\\[") ));
    this->regexes.push_back(regex(leftAngle, makeRegex("^<") )); 
    this->regexes.push_back(regex(rightSquare, makeRegex("^\\]") ));
    this->regexes.push_back(regex(rightAngle, makeRegex("^>") ));
    this->regexes.push_back(regex(colon, makeRegex("^:") ));
    this->regexes.push_back(regex(colonColon, makeRegex("^::") ));
    this->regexes.push_back(regex(comma, makeRegex("^,") ));
    this->regexes.push_back(regex(semiColon, makeRegex("^;") ));
    this->regexes.push_back(regex(equalsSign, makeRegex("^=") ));
    this->regexes.push_back(regex(equalsEquals, makeRegex("^==") ));
    this->regexes.push_back(regex(plusSign, makeRegex("^\\+") ));
    this->regexes.push_back(regex(star, makeRegex("^\\*") ));
    this->regexes.push_back(regex(dash, makeRegex("^\\-") ));
    this->regexes.push_back(regex(percent, makeRegex("^%") ));
    this->regexes.push_back(regex(forwardSlash, makeRegex("^/") ));
    this->regexes.push_back(regex(backSlash, makeRegex("^\\\\") )); 
    this->regexes.push_back(regex(arrow, makeRegex("^->"))) ;
    this->regexes.push_back(regex(lessThanEquals, makeRegex("^<=") ));
    this->regexes.push_back(regex(greaterThanEquals, makeRegex("^>=") ));
    this->regexes.push_back(regex(notEquals, makeRegex("^!=") ));
    this->regexes.push_back(regex(plusPlus, makeRegex("^\\+\\+"))) ;
    this->regexes.push_back(regex(dotDot, makeRegex("^\\.\\.") ));
    // this->regexes.push_back(regex(endOfFile, makeRegex("\\0") ));
};


//here we go...
Token* Scanner::scan(const char* text){
    // If reading in input failed, exit with return code of 1.
     //if (text==NULL) {
     // perror("THIS IS AN EMPTY FILE, DOOFUS."); //does c++ have perror?
     //   exit(1);
     //}
    int Tokennumber = 0;
    int currentSize= 0;
    int matchedcharacter = 0;
    int bigsize = regexes.size();
    //sorta double linked list except taht we only use it when we append the list
    //we return this pointer. Each node we make will have a "next" field, which will point to the next node in the line. 
    Token *head = NULL;
    Token *previous = head;
  

    
    // Consume leading white space and comments

    //I condense it into a new method that returns exactly the same thing.
    // matchedcharacter = consumeWhiteSpaceAndComments(whiteSpace, blockComment, lineComment, text);
    matchedcharacter = consumeWhiteSpaceAndComments(text);
    // printf("\n I have %d\n", matchedcharacter);
    text = text + matchedcharacter;
    // printf("\n we have our guest: '%s'\n", text);

    //we start fresh

    /* text is a character pointer that points to the current
       beginning of the array of characters in the input.  Adding an
       integer value to it advances the pointer that many elements in
       the array.  Thus, text is increased so that it points to the
       current location in the input. 
     */
       //check if everthing is inside the vector
       // for (int t = 0; t < regexes.size(); ++t)
       // {
       //    cout << "so the type in side is " << regexes[t].terminalname<< endl;

       // }

        while ( text[0] != '\0' ) {//until the end of file
        currentSize = 0;
        // printf("\n%s", text);
        Token* temp = new Token(text, lexicalError, NULL );
        // printf("%d\n", regexes.size());
       
        for (unsigned int i = 0; i < bigsize; ++i) {  
            regex object = regexes[i];     
            regex_t *regex = object.expression;
            // printf("\n size of the regexes: %d\n", regexes.size());
            // printf("\nhere we have i: %d\n", i);
            // cout << "so the type in side is " << regexes[i].terminalname<< endl;
            // cout << "let's check whether vector exists: " << regexes[i+25].terminalname<< endl;

              //for each of the terminal, we check whether there is a matched lexeme
            //and the longest matched lexeme is the real token
            matchedcharacter = matchRegex(regex, text);

            // printf("%d\n", matchedcharacter);
            // printf("I matched the shit with the number of: %d \n", matchedcharacter);
            if ( matchedcharacter > currentSize ) {
                currentSize = matchedcharacter;
                temp->terminal = object.terminalname; 
                // printf("\n%d\n", currentSize);
                // printf("how many time we have it? \n");
                //That is kind of strange, I try to case the index number into the Tokenname
                //namely the tokenEnumType
                //if only all these are only object
                // printf("change have been made\n");
                //alternative way to do it is to struct an object which contains
                //index, regex*, and tokenEnumType
               }
               // printf("\nforever? %d\n", i);
               // printf("\n we track i: %d\n", i);
        }
        //it never gets out of the fucking for loop
        // printf("\nwe generate the token \n");
        // cout << "such token is: " << temp->terminal<< endl;
        // cout << "such token is: " << rightSquare<< endl;


        Tokennumber++;
        //we wrap it like an error if it is an error.
        if (temp->terminal == lexicalError) {
            currentSize = 1;
            std::string lexeme (text, currentSize);
            temp->lexeme = lexeme;
            text = text + 1;
            // printf("\nwe almost never enter here\n");
        } else { 
            std::string lexeme (text, currentSize);
            temp->lexeme = lexeme;
            text = text + currentSize;
            // cout << "such lexeme is: " << temp->lexeme<< endl;

        }
        //implement of linkedlist
        //honestly I just hate doing it
        //1902 all over again
        if ( previous != NULL ) {
            previous->next = temp;
        } else {
            head = temp;
        }
        previous = temp;
        matchedcharacter = this->consumeWhiteSpaceAndComments(text);//...and we consume it until the next stop
        text = text + matchedcharacter;

    }

    //Finally we have to put an end to the party
    //And hopfully we can make it here
    //not sure it works but what so fucking ever
    //I wrote it, drunk, at 4 am
    Token *end = new Token("",endOfFile,NULL);
    Tokennumber++;
    end->terminal = endOfFile;
    end->lexeme = "";
    if ( previous != NULL ) {
        previous->next = end;
    } else {
        head = end;
    }

    //return the head, day saved
    // cout << "so the type in side is " << head->terminal << endl;
    return head;
}
        /* maxNumMatchedChars is used to ensure that the regular
           expression that matched the longest string is the one that
           we use.  

           The regexs for word and integerConst cannot match the
           same text, but if we extend this program to search for
           specific keywords, then the keyword regex and the
           word-regex may, in some cases, match the same input text.

           If two regexs match the same number of characters
           then the tie has to be broken.  To break the tie, priority
           is given to the first one that was tried.  Thus the
           comparison
              (numMatchedChars > maxNumMatchedChars) 
           is strictly greater than.  Not greater than or  equal to.
        */


        //ugh, this is going to be long and repetitive. 
        //punctuation first, since it's easiest


        // numMatchedChars = matchRegex (leftParenRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = leftParen ;
  //       }
  //       numMatchedChars = matchRegex (rightParenRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = rightParen ;
  //       }
  //       numMatchedChars = matchRegex (leftSquareRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = leftSquare ;
  //       }
  //       numMatchedChars = matchRegex (rightSquareRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = rightSquare ;
  //       }
  //       numMatchedChars = matchRegex (rightAngleRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = rightAngle ;
  //       }
  //       numMatchedChars = matchRegex (leftAngleRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = leftAngle ;
  //       }
  //       numMatchedChars = matchRegex (colonRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = colon ;
  //       }
  //       numMatchedChars = matchRegex (colonColonRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = colonColon ;
  //       }
  //       numMatchedChars = matchRegex (commaRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = comma ;
  //       }
  //       numMatchedChars = matchRegex (semiColonRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = semiColon ;
  //       }
  //       numMatchedChars = matchRegex (equalsRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = equalsSign ;
  //       }
  //       numMatchedChars = matchRegex (equalsEqualsRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = equalsEquals ;
  //       }
  //       numMatchedChars = matchRegex (plusRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = plusSign ;
  //       }
        // numMatchedChars = matchRegex (starRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = star ;
  //       }

        // numMatchedChars = matchRegex (dashRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = dash ;
  //       }
  //       numMatchedChars = matchRegex (percentRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = percent ;
  //       }
  //       numMatchedChars = matchRegex (forwardslashRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = forwardSlash ;
  //       }
  //       numMatchedChars = matchRegex (backslashRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = backSlash ;
  //       }
  //       numMatchedChars = matchRegex (arrowRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = arrow ;
  //       }
  //       numMatchedChars = matchRegex (lessthanequalsRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = lessThanEquals ;
  //       }
  //       numMatchedChars = matchRegex (greaterthanequalsRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = greaterThanEquals ;
  //       }
  //       numMatchedChars = matchRegex (notEqualsRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = notEquals ;
  //       }
  //       numMatchedChars = matchRegex (plusPlusRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = plusPlus ;
  //       }
  //       numMatchedChars = matchRegex (dotdotRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = dotDot ;
  //       }
  //       numMatchedChars = matchRegex (eofRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = endOfFile ;
  //       }
        
  //       //now for all the stupid keywords. Ugh, this is tedious.
  //       numMatchedChars = matchRegex (letRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = letKwd ;
  //       }
  //       numMatchedChars = matchRegex (inRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = inKwd ;
  //       }
  //       numMatchedChars = matchRegex (endRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = endKwd;
  //       }
  //       numMatchedChars = matchRegex (ifRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = ifKwd;
  //       }
  //       numMatchedChars = matchRegex (thenRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = thenKwd ;
  //       }
  //       numMatchedChars = matchRegex (elseRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = elseKwd ;
  //       }
  //       numMatchedChars = matchRegex (printRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = printKwd ;
  //       }
  //       numMatchedChars = matchRegex (readRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = readKwd ;
  //       }
  //       numMatchedChars = matchRegex (writeRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = writeKwd ;
  //       }
  //       numMatchedChars = matchRegex (integerRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = integerKwd ;
  //       }
  //       numMatchedChars = matchRegex (floatRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = floatKwd;
  //       }
  //       numMatchedChars = matchRegex (booleanRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = booleanKwd ;
  //       }
  //       numMatchedChars = matchRegex (stringRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = stringKwd ;
  //       }
  //       numMatchedChars = matchRegex (trueRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = trueKwd ;
  //       }
  //       numMatchedChars = matchRegex (falseRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = falseKwd ;
  //       }
  //       numMatchedChars = matchRegex (headRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = headKwd ;
  //       }
  //       numMatchedChars = matchRegex (tailRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = tailKwd ;
  //       }
  //       numMatchedChars = matchRegex (nullRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = nullKwd ;
  //       }
  //       numMatchedChars = matchRegex (mapRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = mapKwd ;
  //       }
  //       numMatchedChars = matchRegex (filterRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = filterKwd ;
  //       }
  //       numMatchedChars = matchRegex (foldRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = foldKwd ;
  //       }
  //       numMatchedChars = matchRegex (zipRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal = zipKwd ;
  //       }
        
        
  //      //constants:
  //       numMatchedChars = matchRegex (floatCon, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           matchType = floatConst ;
  //       }    
  //       numMatchedChars = matchRegex (integerConst, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           matchType = intConst ;
  //       }
  //       numMatchedChars = matchRegex (stringConstRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal =  stringConst;
  //       }
        
        
        // //need lexerror

        // numMatchedChars = matchRegex (tupleRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal =  tupleOp;
  //       }
  //       numMatchedChars = matchRegex (variableNameRegex, text) ;
  //       if (numMatchedChars > maxNumMatchedChars) {
  //           maxNumMatchedChars = numMatchedChars ;
  //           terminal =  variableName;
        



// //////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////okay, we need to figure out what to do here - put some sort of set statement inbetween case and break, I'm assuming? see wordcount for structure 


//  //need this switch statement so that we can tell it what to do if none of the regexes match (lex error)
//         switch (terminal) 
//      {
               
//       case letKwd: node.terminal = "letKwd"; break;
//       case inKwd: break; 
//       case endKwd: break;
//       case ifKwd: break;
//       case thenKwd: break;
//       case elseKwd: break; 
//       case printKwd: break;
//       case readKwd: break;
//       case writeKwd: break;  
//       case integerKwd: break;
//       case floatKwd: break;
//       case booleanKwd: break;
//       case stringKwd: break; 
//       case trueKwd: break;
//       case falseKwd: break; 
//       case headKwd: break;
//       case tailKwd: break;
//       case nullKwd: break; 
//       case mapKwd: break;
//       case filterKwd: break;
//       case foldKwd: break; 
//       case zipKwd: break;        
//       case intConst: break;
//       case floatConst: break;
//       case stringConst: break; 
//       case variableName: break;      
//       case leftParen: break;
//       case rightParen: break; 
//       case leftSquare: break;
//       case rightSquare: break;
//       case leftAngle: break;
//       case rightAngle: break;
//       case colon: break;
//       case colonColon: break;
//       case comma: break;
//       case semiColon: break;
//       case equalsSign: break;
//       case plusSign: break;
//       case star: break;
//       case dash: break;
//       case forwardSlash: break;
//       case percent: break; 
//       case backSlash: break;
//       case arrow: break;
//       case equalsEquals: break; 
//       case lessThanEquals: break;
//       case greaterThanEquals: break;
//       case notEquals: break; 
//       case plusPlus: break;
//       case dotDot: break; 
//       case tupleOp: break;
//       case endOfFile: break;
        
       
//      case lexicalError: ; //don't break here, just go on
//         }

//         if (terminal == lexicalError) {
//         // need to throw some sort of error here............
//             // If we didn't match anything, then just skip the first character.
//             text = text + 1 ;
//         }
//         //////////////////////////////////////////////////////////////////////////////////////////////////
//         else {
//             // Consume the characters that were matched.
//             text = text + maxNumMatchedChars ;
//         }

//         // Consume white space and comments before trying again for
//         // another word or integer.
//         numMatchedChars = consumeWhiteSpaceAndComments (whiteSpace, blockComment, lineComment, 
//                                                         text) ;
//         text = text + numMatchedChars ;

//     }

//     /* In this application the only information we collect is the
//        number of words and number of integer constants.  In a scanner
//        we would need to accumulate the list of tokens. */
       
       
//    //pretty sure we can just return head 
//    return head;









// //////////////////////I haven't touched consumeWhiteSpace
// int consumeWhiteSpaceAndComments (regex_t * whiteSpace, 
//                                   regex_t * blockComment, 
//                                regex_t * lineComment,
//                                   const char * text) {
//     int numMatchedChars = 0 ;
//     int totalNumMatchedChars = 0 ;
//     int stillConsumingWhiteSpace ;

//     do {
//         stillConsumingWhiteSpace = 0 ;  // exit loop if not reset by a match

//         // Try to match white space
//         numMatchedChars = matchRegex (whiteSpace, text) ;
//         totalNumMatchedChars += numMatchedChars ;
//         if (numMatchedChars > 0) {
//             text = text + numMatchedChars ;
//             stillConsumingWhiteSpace = 1 ;
//         }

//         // Try to match block comments
//         numMatchedChars = matchRegex (blockComment, text) ;
//         totalNumMatchedChars += numMatchedChars ;
//         if (numMatchedChars > 0) {
//             text = text + numMatchedChars ;
//             stillConsumingWhiteSpace = 1 ;
//         }

//         // Try to match single-line comments
//         numMatchedChars = matchRegex (lineComment, text) ;
//         totalNumMatchedChars += numMatchedChars ;
//         if (numMatchedChars > 0) {
//             text = text + numMatchedChars ;
//             stillConsumingWhiteSpace = 1 ;
//         }
//     }
//     while ( stillConsumingWhiteSpace ) ;    

//     return totalNumMatchedChars ;
// }


