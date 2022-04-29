#include "secd.h"
#include "stack.cxx"

class SECD {
    /* Upon instantiating new SECD, each S, E, C, D register
       contains a new stack with "NIL" as head.
    */
    public:
        Stack * stack = new Stack();
        Stack * environment = new Stack();
        Stack * control = new Stack();
        Stack * dump = new Stack();
    
    
};