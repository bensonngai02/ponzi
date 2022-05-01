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

void SECD::math(std::string operation) {
    peekStackExpType(stack, ATOM_TYPE);
    Atom* op1 = (Atom*) Node::pop(&stack);
    peekStackExpType(stack, ATOM_TYPE);
    Atom* op2 = (Atom*) Node::pop(&stack);
    
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

void SECD::execute(Node * control) {
    Expression * inst = Node::pop(&control)->car();
    inst->print();
    control->print();
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
        Expression* op1 = Node::pop(&stack);
        Node::push(&stack, op1);
    }
}

void SECD::peekStackExpType(Node * stack, int expType) {
    if (stack->peek()->car()->getExpType() != expType) {
        std::cout << "Top of stack is not atom";
        exit(1);
    }
}


int main(){
    std::string str2 = createInterpretedString();
    int i = 0;
    Node* parsed = (Node*) consume(&str2, &i);
    std::cout << "Parsed: ";
    parsed->print();
    std::cout << "CAR: ";
    parsed->car()->print();
    std::cout << "CDR: ";
    parsed->cdr()->print();
    SECD * secd = new SECD();
    for(int i = 0; i < 4; i++){
        secd->execute(parsed);
    }
    
}