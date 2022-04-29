#include "secd.h"
#include "expression.cxx"

class SECD {
    /* Upon instantiating new SECD, each S, E, C, D register
       contains a new stack with "NIL" as head.
    */
    public:
        Expression * stack = new Expression();
        Expression * environment = new Expression();
        Expression * control = new Expression();
        Expression * dump = new Expression();
    
    
};