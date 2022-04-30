#include "secd.h"
#include "expression.h"
#include "node.h"
#include "atom.h"
#include "intermediateParser.cxx"

/* Upon instantiating new SECD, each S, E, C, D register
    contains a new stack with "NIL" as head.
*/

std::string inputSStr = createInterpretedString();
std::string* input = &inputSStr;
int i = 0;

// all instantiated to (NIL, NIL)
Node * stack = new Node();
Node * environment = new Node();
// need to read in control 
Node * control = (Node *) getControlPtr(input, &i);
Node * dump = new Node();

void execute(Node * inst_node) {
    Expression * inst = inst_node->car();
    int inst_type = inst->getType();
    if (inst_type == NIL_TYPE) {
        // stop the program
        //above comment is wrong
    }
    else if (inst_type == ATOM_TYPE) {

    }
    else if (inst_type == NODE_TYPE) {

    }

}