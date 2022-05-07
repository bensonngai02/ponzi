#include "node.h"
#include "secd.h"

Node::Node() {
    this->carPtr = (Expression *) new Atom();
    this->cdrPtr = (Expression *) new Atom();
    this->expType = NODE_TYPE;
    this->rplacable = true;
}

Node::Node(Atom * a1, Atom * a2){
    this->carPtr = (Expression *) a1;
    this-> cdrPtr = (Expression *) a2;
    this->expType = NODE_TYPE;
    this->rplacable = false;
}
Node::Node(Atom * a1, Node * n2){
    this->carPtr = (Expression *) a1;
    this->cdrPtr = (Expression *) n2;
    this->expType = NODE_TYPE;
    this->rplacable = false;
}
Node::Node(Node * n1, Atom * a2){
    this->carPtr = (Expression *) n1;
    this->cdrPtr = (Expression *) a2;
    this->expType = NODE_TYPE;
    this->rplacable = false;
}
Node::Node(Node * n1, Node * n2){
    this->carPtr = (Expression *) n1;
    this->cdrPtr = (Expression *) n2;
    this->expType = NODE_TYPE;
    this->rplacable = false;
}

Expression * Node::car() {
    return this->carPtr;
}
Expression * Node::cdr() {
    return this->cdrPtr;
}

void Node::setCar(Expression * c) {
    this->carPtr = c;
}

void Node::setCdr(Expression * c) {
    this->cdrPtr = c;
}

/* returns copy of a node to cons (immutability) */
Expression * Node::copy(){
    return Node::cons(this->car()->copy(), this->cdr()->copy());
}

/* Takes in two node/atom arguments and combines them to create new node in list */
Node * Node::cons(Expression * car, Expression * cdr) {
    Node * newNode;
    if(cdr->expType == NODE_TYPE && ((Node*) cdr)->rplacable){
        ((Node*)cdr)->rplaca(car);
        return (Node*) cdr;
    }
    int car_type = car->expType;
    int cdr_type = cdr->expType;
    if (car_type == ATOM_TYPE && cdr_type == ATOM_TYPE)
        newNode = new Node((Atom *) car, (Atom *) cdr);
    else if (car_type == ATOM_TYPE && cdr_type == NODE_TYPE)
        newNode = new Node((Atom *) car, (Node *) cdr);
    else if (car_type == NODE_TYPE && cdr_type == ATOM_TYPE)
        newNode = new Node((Node *) car, (Atom *) cdr);
    else
        newNode = new Node((Node *) car, (Node *) cdr);
    return newNode;
}

void Node::push(Node** headPtr, Expression * expression) {
    *headPtr = cons(expression, *headPtr);
}

Expression * Node::pop(Node** headPtr) { 
    Node * head = *headPtr;
    *headPtr = (Node *) head->cdr(); 
    // if NIL type, head->cdr() as performed above doesn't work for NIL type
    // check is necessary to instantiate new atom representing NIL type
    if((*headPtr)->getExpType() == NIL_TYPE){
        *headPtr = new Node();
    }
    head->cdrPtr = new Atom();
    return head->car();
}

Expression * Node::peek() {
    return this;
}

/* replaces the CAR (head of expression) */
void Node::rplaca(Expression* c){
    this->carPtr = c;
    this->rplacable = false;
}

bool Node::getRplacable() {
    return rplacable;
}

void Node::printRecur(){
    printf("(");
    this->carPtr->printRecur();
    this->cdrPtr->printRecur();
    printf(")");
}

void Node::print(){
    this->printRecur();
    std::cout << std::endl;
}
