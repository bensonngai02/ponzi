#include "secd.h"
#include "expression.cxx"
#include "node.cxx"
#include "node.h"

class SECD {
    /* Upon instantiating new SECD, each S, E, C, D register
       contains a new stack with "NIL" as head.
    */
    public:
        Node * stack = new Node();
        Node * environment = new Node();
        Node * control = new Node();
        Node * dump = new Node();
    
    
};