#include "secd.h"
#include "expression.h"
#include "node.h"
#include "atom.h"
#include "intermediateParser.cxx"

/* Upon instantiating new SECD, each S, E, C, D register
    contains a new stack with "NIL" as head.
*/

std::string * input = createInterpretedString();
int i = 0;

// all instantiated to (NIL, NIL)
Node * stack = new Node();
Node * environment = new Node();
Node * control = (Node *) getControlPtr(input, &i);
Node * dump = new Node();






// need to read in control 