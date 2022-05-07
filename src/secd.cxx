#include "secd.h"
#include "expression.h"
#include "atom.h"
#include "parser.h"
#include "node.h"

#include <iostream>
#include <fstream>

/* Upon instantiating new SECD, each S, E, C, D register
    contains a new stack with "NIL" as head.
*/

SECD::SECD(std::string inputFile){
    int i = 0;
    std::string inputSStr = createInterpretedString(inputFile);
    stack = new Node();
    environment = new Node();
    // need to read in control 
    control = (Node *) getControlPtr(&inputSStr, &i);
    dump = new Node();
}

SECD::SECD(Node* con){
    stack = new Node();
    environment = new Node();
    // need to read in control 
    control = con;
    dump = new Node();
}

void SECD::peekStackExpType(Node * stack, int expType) {
    if (stack->peek()->car()->getExpType() != expType) {
        std::cout << "Top of stack is not atom";
        exit(1);
    }
}

Expression * SECD::index(Atom* n, Node * list) {
    if (Node::eq(n, new Atom(0))) {
        std::cout << "Returing: " << list->cadr()->getExpType();
        list->car()->print();
        return list->car();
    }
    else {
        Atom* temp = new Atom(n->get_atom_integer() - 1);
        list->print();
        return SECD::index(temp, ((Node *) list->cdr()));
    }
}

Expression * SECD::locate(Node * coordinates, Node * environment) { //wrong
    if(coordinates->car()->getExpType() != ATOM_TYPE || coordinates->cadr()->getExpType() != ATOM_TYPE){
        std::cout << "Calling location on a non-2-tuple";
        std::cout << "Car: ";
        coordinates->car()->print();
        std::cout << "Cadr: ";
        coordinates->cadr()->print();
        exit(1);
    }
    Atom * b = (Atom*) coordinates->car();
    Atom * n = (Atom*) coordinates->cadr();
    Expression* ret = index(n, (Node*) index(b, environment));
    ret->print();
    return ret;
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
        Boolean bo = equals ? t: f;
        result = new Atom(bo);
    }
    Node::push(&stack, result);
}

void SECD::execute() {
    Expression * inst = Node::pop(&control)->car();
    int inst_type = inst->getExpType();
    if (inst_type != ATOM_TYPE) {
        std::cout << "Trying to execute a non instruction expression: " ;
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
        Expression* op1 = Node::pop(&control);
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
                std::cout << "SEL on non-node\n";
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
        std::cout << "------------------------------------" << std::endl;
        std::cout << "OUTPUT" << std::endl;
        std::cout << "------------------------------------" << std::endl;
        std::cout << "Stack: ";
        stack->print();
        std::cout << "Environment: ";
        environment->print();
        std::cout << "Control: ";
        control->print();
        std::cout << "Dump: ";
        dump->print();
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
        Expression* result = Node::cons(op2, op1);
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
        // Expression* tempStack = Node::pop(&dump);
        // Expression* tempEnv = Node::pop(&dump);
        // Expression* tempControl = Node::pop(&dump);
        // if(tempStack->getExpType() != NODE_TYPE || 
        //     tempEnv->getExpType() != NODE_TYPE || 
        //     tempControl->getExpType() != NODE_TYPE){
        //     std::cout << "RET prev values is not Node";
        //     std::cout << tempStack->getExpType() << " " << tempEnv->getExpType() << " " << tempControl->getExpType() << std::endl;
        //     tempStack->print();
        //     tempEnv->print();
        //     tempControl->print();
        //     exit(1);
        // }
        // Node* prevStack = (Node*) tempStack;
        // environment = (Node*) tempEnv;
        // control = (Node*) tempControl;
        // Node::push(&prevStack, stack->car());
        // stack = prevStack;
        stack = Node::cons(stack->car(), dump->car());
        environment = (Node*) dump->cadr();
        control = (Node* ) dump->caddr();
        dump = (Node*) dump->cddr()->cdr();
    }
    else if (atomInstString == "DUM") {
        Node * emptyList = new Node(); // TODO: Check
        Node::push(&environment, emptyList);
    }
    else if (atomInstString == "LD") {
        Node * locationNode = (Node *) Node::pop(&control);
        Expression * variable = locate(locationNode, environment);
        Node::push(&stack, variable);
    }
    else if (atomInstString == "LDF") {
        Node * function = (Node *) Node::pop(&control);
        Node * closure = Node::cons(function, environment->copy());
        Node::push(&stack, closure);
    }
    else if (atomInstString == "AP") {
        Node * function = (Node *) Node::pop(&stack);
        Node * parameters = (Node *) Node::pop(&stack);
        Node::push(&dump, control);
        Node::push(&dump, environment);
        Node::push(&dump, stack);
        Node* newControl = (Node*) function->car();
        // std::cout << "Function cdr: ";
        // function->cdr()->print();
        parameters->print();
        Node* newEnv = Node::cons(parameters, function->cdr());
        // std::cout << "NewEnv: ";
        // newEnv->print();
        control = newControl;
        environment = newEnv;
        stack = new Node(); //TODO: Check
    }
    else if (atomInstString == "RAP") {
        // Node* closure = (Node*) Node::pop(&stack);
        // Expression* v = Node::pop(&stack);
        // // pop dummy environment off stack
        // Expression* omega = environment->peek();
        // Node* newNode = new Node();
        // // std::cout << omega->getExpType() << omega->car()->getExpType() << omega->cdr()->getExpType();
        // if(omega->car()->getExpType() != 3 && omega->cdr()->getExpType() != 3){
        //     std::cout << "RAP cannot be executed because the top of the enivronment stack is not omega";
        //     exit(1);
        // }
        // Node::push(&dump, control);
        // Node::push(&dump, environment);
        // Node::push(&dump, stack);
        // stack = new Node();
        // control = (Node*) closure->car();
        // environment = (Node*) closure->cdr();   // omega should be first item of closure->cdr()
        // std::cout << "Environment' first item: ";
        // environment->car()->print();
        // environment->rplaca(v);

        dump = Node::cons(stack->cddr(), Node::cons(environment->cdr(), Node::cons(control, dump)));
        environment = (Node *) stack->cdar();
        environment->rplaca(stack->cadr());
        control = (Node *) stack->caar();
        stack = new Node();
    }
    else if (atomInstString == "WRITE") {
        Node * output = (Node *) Node::pop(&stack);
        output->print();
    }
    else if (atomInstString == "FINN") {
        std::ifstream finn;
        finn.open("docs/finn.txt");
        std::string line;
        if (finn.is_open()) {
            while(getline(finn, line)) {
                std::cout << line << std::endl;
            }
            finn.close();
        }
    }
    else if (atomInstString == "LEO") {
        std::ifstream leo;
        leo.open("docs/leo.txt");
        std::string line;
        if (leo.is_open()) {
            while(getline(leo, line)) {
                std::cout << line << std::endl;
            }
            leo.close();
        }
    }
    else if (atomInstString == "NIKITA") {
        std::cout << "|     ALL HAIL PIAZZA PROFILE PICS     |" << std::endl;
        std::ifstream nikita;
        nikita.open("docs/nikita.txt");
        std::string line;
        if (nikita.is_open()) {
            while(getline(nikita, line)) {
                std::cout << line << std::endl;
            }
            nikita.close();
        }
    }
    else if (atomInstString == "MICHAEL") {
        std::ifstream michael;
        michael.open("docs/michael.txt");
        std::string line;
        if (michael.is_open()) {
            while(getline(michael, line)) {
                std::cout << line << std::endl;
            }
            michael.close();
        }
    }
}

/* Checks whether a target node is in a list recursively */
bool SECD::member(Node * target, Node * list) {
    if (list->getExpType() == NIL_TYPE)
        return false;
    if (Node::eq(target, list->car()))
        return true;
    else 
        return member(target, (Node *) list->cdr());
}

/* Returns the position of the target in the list recursively */
int SECD::position(Node * target, Node * list) {
    bool val_eq = Node::eq(target, list->car());
    return val_eq? 0 : 1 + position(target, (Node *) list->cdr());
}

/* Returns a node of the location of the target in the list in the form of a coordinate pair */
Node * SECD::location(Node * target, Node * list) {
    if(Node::eq(new Atom(), list->car())){
            std::cout << "Reached end of list without finding location. Printing environment list: " << std::endl;
            exit(1);
        }
    if (member(target, (Node *) list->car())) {
        // If a member of the list, x-coordinate must be 0. 
        int got_pos = position(target, (Node *) list->car());
        Atom * car_atom = new Atom(0);
        // Find y coordinate
        Atom * cdr_atom = new Atom(got_pos);
        return Node::cons(car_atom, cdr_atom);
    }
    else {
        Node * z = (SECD::location(target, (Node *) list->cdr()));
        Atom * car_z = (Atom *) z->car();
        Atom * cdr_z = (Atom *) z->cdr();
        Atom * new_car = new Atom(car_z->get_atom_integer() + 1);
        Atom * new_cdr = new Atom(cdr_z->get_atom_integer());
        return Node::cons(new_car, new_cdr);
    }
}


void SECD::print(){
    std::cout << &stack << &environment << &control << &dump << std::endl;
    std::cout << "Stack: ";
    stack->print();

    std::cout << "Environment: ";
    environment->print();

    std::cout << "Control: ";
    control->print();

    std::cout << "Dump: ";
    dump->print();

    std::cout << std::endl;
}

// int main(int argc, char** argv){
//     SECD * secd = new SECD(argv[1]);
//     while(true){
//         secd->print();
//         secd->execute();
//     }
// }
