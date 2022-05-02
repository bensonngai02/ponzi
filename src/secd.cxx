#include "secd.h"
#include "expression.h"
#include "atom.h"
#include "intermediateParser.h"
/* Upon instantiating new SECD, each S, E, C, D register
    contains a new stack with "NIL" as head.
*/

SECD::SECD(){
    i = 0;
    inputSStr = createInterpretedString();
    input = &inputSStr;
    stack = new Node();
    environment = new Node();
    // need to read in control 
    control = (Node *) getControlPtr(input, &i);
    dump = new Node();
}

void SECD::peekStackExpType(Node * stack, int expType) {
    if (stack->peek()->car()->getExpType() != expType) {
        std::cout << "Top of stack is not atom";
        exit(1);
    }
}
void SECD::mathOp(std::string operation) {
    peekStackExpType(stack, ATOM_TYPE);
    Atom* op2 = (Atom*) Node::pop(&stack);
    peekStackExpType(stack, ATOM_TYPE);
    Atom* op1 = (Atom*) Node::pop(&stack);
    
    Atom* result;
    if (operation == "ADD")
        result = Atom::add(op1,op2);
    else if (operation == "SUB")
        result = Atom::sub(op1,op2);
    else if (operation == "MUL")
        result = Atom::mul(op1, op2);
    else if (operation == "DIV")
        result = Atom::div(op1, op2);
    else
        result = Atom::rem(op1, op2);

    Node::push(&stack, result);
}

void SECD::boolOp(std::string operation) {
    peekStackExpType(stack, ATOM_TYPE);
    Atom* op2 = (Atom*) Node::pop(&stack);
    peekStackExpType(stack, ATOM_TYPE);
    Atom* op1 = (Atom*) Node::pop(&stack);

    Atom * result;

    if (operation == "GT")
        result = Atom::gt(op1, op2);
    else if (operation == "LT")
        result = Atom::lt(op1, op2);
    else if (operation == "GEQ")
        result = Atom::geq(op1, op2);
    else if (operation == "LEQ")
        result = Atom::leq(op1, op2);
    else if (operation == "EQ") {
        bool equals = Expression::eq(op1, op2);
        result = new Atom(equals);
    }
    Node::push(&stack, result);
}

void SECD::execute() {
    Expression * inst = Node::pop(&control)->car();
    int inst_type = inst->getExpType();
    if (inst_type != ATOM_TYPE) {
        std::cout << "Trying to execute a non instruction expression: ";
        inst->print();
        exit(1);
    }

    Atom * atom_inst = (Atom *) inst;
    // consider integer, string, boolean cases
    std::string atomInstString = atom_inst->get_atom_string();

    if (atomInstString == "ADD")
        mathOp("ADD");
    else if (atomInstString== "SUB")
        mathOp("SUB");
    else if (atomInstString == "MUL")
        mathOp("MUL");
    else if (atomInstString == "DIV")
        mathOp("DIV");
    else if (atomInstString == "REM")
        mathOp("REM");
    else if (atomInstString == "EQ")
        boolOp("EQ");
    else if (atomInstString == "GT")
        boolOp("GT");
    else if (atomInstString == "LT")
        boolOp("LT");
    else if (atomInstString == "GEQ")
        boolOp("GEQ");
    else if (atomInstString == "LEQ")
        boolOp("LEQ");
    else if (atomInstString == "NULL") {
        bool eq = stack->peek()->car()->getExpType() == NIL_TYPE;
        Boolean val = eq ? t : f;
        Node::push(&stack, new Atom(val));
    }
    else if (atomInstString == "NIL") {
        Node::push(&stack, new Atom());
    }
    else if (atomInstString == "LDC"){
        Expression* op1 = Node::pop(&control)->car();
        Node::push(&stack, op1);
    }
    else if (atomInstString == "SEL") {
        Atom * popped = (Atom *) Node::pop(&stack);
        bool cond = popped->get_atom_boolean();
        Expression * ct = Node::pop(&control);
        Expression * cf = Node::pop(&control);
        Node::push(&dump, control);
        if (cond) {
            // if val = 1 -> replace control w 'ct'
            if (ct->getExpType() != NODE_TYPE){
                std::cout << "SEL on non node";
                exit(1);
            }
            control = (Node *) ct;
        }
        else {
            // if val = 0 -> replace control w 'cf'
            if (cf->getExpType() != NODE_TYPE){
                std::cout << "SEL on non node";
                exit(1);
            }
            control = (Node *) cf;
        }
    }
    else if (atomInstString == "STOP") {
        // printStack(stack);
        std::cout << "Stack is: ";
        stack->print();
        std::cout << std::endl;
        exit(1);
    }
    else if(atomInstString == "CAR"){
        Expression* op1 = Node::pop(&stack)->car();
        Node::push(&stack, op1);
    }
    else if(atomInstString == "CDR"){
        Expression* op1 = Node::pop(&stack)->cdr();
        Node::push(&stack, op1);
    }
    else if(atomInstString == "CONS"){
        Expression* op2 = Node::pop(&stack);
        Expression* op1 = Node::pop(&stack);
        Expression* result = Node::cons(op1, op2);
        Node::push(&stack, result);
    }
    else if(atomInstString == "ATOM"){
        Expression* op1 = Node::pop(&stack);
        bool b = op1->getExpType() == ATOM_TYPE;
        Boolean bo = b ? t : f;
        Atom* result = new Atom(bo);
        Node::push(&stack, result);
    }
    else if (atomInstString == "JOIN") {
        if (dump->peek()->getExpType() != NODE_TYPE) {
            std::cout << "JOIN on non node (top of dump is not a node when it should be).";
            exit(1);
        }
        control = (Node *) Node::pop(&dump);
    }
    else if (atomInstString == "RET") {
        Expression* tempStack = Node::pop(&dump);
        Expression* tempEnv = Node::pop(&dump);
        Expression* tempControl = Node::pop(&dump);
        if(tempStack->getExpType() != NODE_TYPE || 
            tempEnv->getExpType() != NODE_TYPE || 
            tempControl->getExpType() != NODE_TYPE){
            std::cout << "RET prev values is not Node";
            exit(1);
        }
        Node* prevStack = (Node*) tempStack;
        environment = (Node*) tempEnv;
        control = (Node*) tempControl;
        Node::push(&prevStack, stack);
        stack = prevStack;
    }
    else if (atomInstString == "DUM") {
        Node * emptyList = Node::cons(new Atom(), new Atom()); // might be buggy
        Node::push(&environment, emptyList);
    }
   
    else if (atomInstString == "LD") {
        Node * locationNode = (Node *) Node::pop(&stack);
        Node * variables = Node::location(locationNode, control);
        Node::push(&stack, variables);
    }
    else if (atomInstString == "LDF") {
        Node * function = (Node *) Node::pop(&control);
        Node * closure = Node::cons(function, environment->copy());
        Node::push(&stack, closure);
    }
    else if (atomInstString == "AP") {
        Node * function = (Node *) Node::pop(&stack);
        Node * parameters = (Node *) Node::pop(&stack);
        // to-do: install environment as the current one (load parameters)
        Node::push(&dump, control);
        Node::push(&dump, environment);
        Node::push(&dump, stack);
        Node* newControl = (Node*) function->car();
        Node* newEnv = Node::cons(parameters, function->cdr());
        control = newControl;
        environment = newEnv;
        stack = Node::cons(new Atom(), new Atom());
    }
    else if (atomInstString == "RAP") {
        Node* closure = (Node*) Node::pop(&stack);
        Expression* v = Node::pop(&stack);
    }
    else if (atomInstString == "WRITEC") {
        Node * output = (Node *) Node::pop(&control);
        output->print();
    }
}

int main(){
    SECD * secd = new SECD();
    secd->stack->print();
    while(true){
        std::cout << "Control: ";
        secd->control->print();
        std::cout << "Stack: ";
        secd->stack->print();
        secd->execute();
    }
}