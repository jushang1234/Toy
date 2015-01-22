#ifndef Node_H
#define Node_H

#include <string>
#include <iostream> 
#include <list>

#include "scanner.h"


// Node
class Node {
public:
    virtual std::string unparse ( ) = 0 ;
//    virtual std::string cppCode ( ) = 0 ; //-TODO iter4 
    virtual ~Node() { }
} ;


#endif // Node_H
