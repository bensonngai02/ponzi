#include "secd.h"
#include "expression.h"
#include "atom.h"
#include "parser.h"
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
        std::cout << "Stack is: ";
        stack->print();
        std::cout << "Environment is: ";
        environment->print();
        std::cout << "Control is: ";
        control->print();
        std::cout << "Dump is: ";
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
        Node * emptyList = Node::cons(new Atom(1), new Atom(1)); // TODO: Check
        Node::push(&environment, emptyList);
    }
    else if (atomInstString == "LD") {
        Node * locationNode = (Node *) Node::pop(&control);
        Node * variables = Node::location(locationNode, environment);
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
        Node::push(&dump, control);
        Node::push(&dump, environment);
        Node::push(&dump, stack);
        Node* newControl = (Node*) function->car();
        Node* newEnv = Node::cons(parameters, function->cdr());
        control = newControl;
        environment = newEnv;
        stack = new Node(); //TODO: Check
    }
    else if (atomInstString == "RAP") {
        Node* closure = (Node*) Node::pop(&stack);
        Expression* v = Node::pop(&stack);
        // pop dummy environment off stack
        Expression* omega = Node::pop(&environment);
        if(!Node::eq(omega, new Node())){
            std::cout << "RAP cannot be executed because the top of the enivronment stack is not omega";
            exit(1);
        }
        Node::push(&dump, control);
        Node::push(&dump, environment);
        Node::push(&dump, stack);
        stack = new Node();
        control = (Node*) closure->car();
        environment = (Node*) closure->cdr();   // omega should be first item of closure->cdr()
        std::cout << "Environment' firt item: ";
        environment->car()->print();
        environment->rplaca(v);
    }
    else if (atomInstString == "WRITEC") {
        Node * output = (Node *) Node::pop(&stack);
        output->print();
    }
    else if (atomInstString == "MICHAEL") {
        std::cout << "Ode to MJ" << std::endl;
        std::cout << "He sing" << std::endl;
        std::cout << "He dance" << std::endl;
        std::cout << "But most importantly" << std::endl;
        std::cout << "He hee" << std::endl;
        std::cout << "https://www.youtube.com/watch?v=GCUz359flrM" <<std::endl;
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

int main(int argc, char** argv){
    SECD * secd = new SECD(argv[1]);
    while(true){
        secd->print();
        secd->execute();
    }
}