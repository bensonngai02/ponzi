#include "secd.h"
#include "expression.h"
#include "node.h"
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

void SECD::math(std::string operation) {
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

    if (atom_inst->get_atom_string() == "ADD")
        math("ADD");
    else if (atom_inst->get_atom_string() == "SUB")
        math("SUB");
    else if (atom_inst->get_atom_string() == "MUL")
        math("MUL");
    else if (atom_inst->get_atom_string() == "DIV")
        math("DIV");
    else if (atom_inst->get_atom_string() == "REM")
        math("REM");
    else if (atom_inst->get_atom_string() == "STOP") {
        // printStack(stack);
        std::cout << "Stack is: ";
        stack->print();
        std::cout << std::endl;
        exit(1);
    }
    else if(atom_inst->get_atom_string() == "LDC"){
        Expression* op1 = Node::pop(&control)->car();
        Node::push(&stack, op1);
    }
}


int main(){
    SECD * secd = new SECD();
    std::cout << "Initial Control: ";
    secd->control->print();
    std::cout << "Initial Stack: ";
    secd->stack->print();
    for(int i = 0; i < 4; i++){
        secd->execute();
        std::cout << "Control: ";
        secd->control->print();
        std::cout << "Stack: ";
        secd->stack->print();
    }
}