#include "secd.h"
#include "expression.h"
#include "atom.h"
#include "parser.h"
#include "node.h"

#include <iostream>
#include <fstream>

/* Upon instantiating new SECD, each S, E, C, D register
    contains a new stack with "NIL" as head. */
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
        Atom* temp = new Atom(n->getAtomInteger() - 1);
        list->print();
        return SECD::index(temp, ((Node *) list->cdr()));
    }
}

Expression * SECD::locate(Node * coordinates, Node * environment) { //wrong
    if(coordinates->car()->getExpType() != ATOM_TYPE || coordinates->cadr()->getExpType() != ATOM_TYPE){
        std::cout << "Calling location on a non-2-tuple";
        std::cout << "CAR: ";
        coordinates->car()->print();
        std::cout << "CADR: ";
        coordinates->cadr()->print();
        exit(1);
    }
    Atom * b = (Atom*) coordinates->car();
    Atom * n = (Atom*) coordinates->cadr();
    Expression* ret = index(n, (Node*) index(b, environment));
    ret->print();
    return ret;
}

/* handles all arithmetic operations with two operands 
   (add, sub, mul, div, rem) */
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

/* handles all boolean operations with two operands 
   (gt, lt, geq, leq, eq) */
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

/* executes a ponzi instruction part of our established ISA */
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
    std::string atomInstString = atom_inst->getAtomString();
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
    // NIL - pushes NIL onto stack
    else if (atomInstString == "NIL") {
        Node::push(&stack, new Atom());
    }
    // LDC - loads constant from control flow to stack
    else if (atomInstString == "LDC"){
        Expression* op1 = Node::pop(&control);
        Node::push(&stack, op1);
    }
    // SEL - if top of stack is 1, runs If-statement. Else, runs Else-statement.
    else if (atomInstString == "SEL") {
        Atom * popped = (Atom *) Node::pop(&stack);
        bool cond = popped->getAtomBoolean();
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
    // STOP - ends execution of program
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
    // CAR - gets the head of list
    else if(atomInstString == "CAR"){
        Expression* op1 = Node::pop(&stack)->car();
        Node::push(&stack, op1);
    }
    // CDR - gets the tail of list (rest of list besides first item (atom/node))
    else if(atomInstString == "CDR"){
        Expression* op1 = Node::pop(&stack)->cdr();
        Node::push(&stack, op1);
    }
    // CONS - pops two items off stack and combines them, pushing result to stack
    else if(atomInstString == "CONS"){
        Expression* op2 = Node::pop(&stack);
        Expression* op1 = Node::pop(&stack);
        Expression* result = Node::cons(op2, op1);
        Node::push(&stack, result);
    }
    // ATOM - checks whether top of stack is an atom type, pushing result to stack
    else if(atomInstString == "ATOM"){
        Expression* op1 = Node::pop(&stack);
        bool b = op1->getExpType() == ATOM_TYPE;
        Boolean bo = b ? t : f;
        Atom* result = new Atom(bo);
        Node::push(&stack, result);
    }
    // JOIN - returns to default program control flow after If-Else statement is run
    else if (atomInstString == "JOIN") {
        if (dump->peek()->getExpType() != NODE_TYPE) {
            std::cout << "JOIN on non-node (top of dump is not a node when it should be).";
            exit(1);
        }
        control = (Node *) Node::pop(&dump);
    }
    // RET - return needed at end of function code to return out of function
    // needed to restore stack, environment, and control stacks previously stored on dump
    else if (atomInstString == "RET") {
        stack = Node::cons(stack->car(), dump->car());
        environment = (Node*) dump->cadr();
        control = (Node* ) dump->caddr();
        dump = (Node*) dump->cddr()->cdr();
    }
    // DUM - pushes dummy environment, needed for recursively apply
    else if (atomInstString == "DUM") {
        environment = Node::cons(new Atom(), environment);
        control = (Node *) control->cdr();
    }
    // LD - loads variables from a specific environment
    else if (atomInstString == "LD") {
        Node * locationNode = (Node *) Node::pop(&control);
        Expression * variable = locate(locationNode, environment);
        Node::push(&stack, variable);
    }
    // LD - loads function by creating closure with function code + environment,
    //      pushing the closure (pair) to stack
    else if (atomInstString == "LDF") {
        Node * function = (Node *) Node::pop(&control);
        Node * closure = Node::cons(function, environment->copy());
        Node::push(&stack, closure);
    }
    // AP - applies function with parameters located directly below function code in stack
    //      saves state of S, E, C onto dump before function application
    //      sets new state of S, E, C with locally scoped data in function (parameters)
    else if (atomInstString == "AP") {
        Node * function = (Node *) Node::pop(&stack);
        Node * parameters = (Node *) Node::pop(&stack);
        Node::push(&dump, control);
        Node::push(&dump, environment);
        Node::push(&dump, stack);
        Node* newControl = (Node*) function->car();
        parameters->print();
        Node* newEnv = Node::cons(parameters, function->cdr());
        control = newControl;
        environment = newEnv;
        stack = new Node(); 
    }
    // RAP - recursively apply functions
    else if (atomInstString == "RAP") {
        dump = Node::cons(stack->cddr(), Node::cons(environment->cdr(), Node::cons(control, dump)));
        environment = (Node *) stack->cdar();
        environment->rplaca(stack->cadr());
        control = (Node *) stack->caar();
        stack = new Node();
    }
    // WRITE - modified write instruction to write S-Expression from given Node on top of stack
    else if (atomInstString == "WRITE") {
        Node * output = (Node *) Node::pop(&stack);
        output->print();
    }
    // TA Instructions print images representing each TA. ASCII art in .txt file located in /docs
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
        // finds the location coordinate pair if not found in head of member list
        // acts as recursive step to iterate through environments to look for correct location
        Node * z = (SECD::location(target, (Node *) list->cdr()));
        Atom * car_z = (Atom *) z->car();
        Atom * cdr_z = (Atom *) z->cdr();
        Atom * new_car = new Atom(car_z->getAtomInteger() + 1);
        Atom * new_cdr = new Atom(cdr_z->getAtomInteger());
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

/* The main here is needed if you want to run just the .ponzi file
   If that's the case, the other main in compiler.cxx will have to be commented out */

// int main(int argc, char** argv){
//     SECD * secd = new SECD(argv[1]);
//     while(true){
//         secd->print();
//         secd->execute();
//     }
// }