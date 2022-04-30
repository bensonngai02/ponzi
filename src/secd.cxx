#include "secd.h"
#include "expression.h"
#include "node.h"
#include "atom.h"

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