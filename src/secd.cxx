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

void SECD::execute(Node * control) {
    Expression * inst = Node::pop(&control)->car();
    inst->print();
    std::cout << std::endl;
    control->print();
    std::cout <<std::endl;
    int inst_type = inst->getExpType();
    if (inst_type != ATOM_TYPE) {
        std::cout << "Trying to execute a non instruction expression: ";
        inst->print();
        exit(1);
    }

    Atom * atom_inst = (Atom *) inst;
    // consider integer, string, boolean cases
    if (atom_inst->get_atom_string() == "ADD") {   
        if(stack->peek()->car()->getExpType() != ATOM_TYPE) {
            std::cout << "Top of stack is not atom for add";
            exit(1);
        }
        Atom* op1 = (Atom*) Node::pop(&stack);
        if(stack->peek()->car()->getExpType() != ATOM_TYPE) {
            std::cout << "Top of stack 2 is not atom for add";
            exit(1);
        }
        Atom* op2 = (Atom*) Node::pop(&stack);
        if(op1->getType() != INTEGER || op2->getType() != INTEGER) {
            std::cout << "Operand values are not integer";
            exit(1);
        }
        Atom* result = op1->add(op2);
        Node::push(&stack, result);
    }
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

int main(){
    std::string str2 = createInterpretedString();
    int i = 0;
    Node* parsed = (Node*) consume(&str2, &i);
    parsed->print();
    parsed->car()->print();
    parsed->cdr()->print();
    SECD * secd = new SECD();
    for(int i = 0; i < 4; i++){
        secd->execute(parsed);
    }
    
}