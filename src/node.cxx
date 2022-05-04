#include "node.h"
#include "secd.h"

Node::Node() {
    this->carPtr = (Expression *) new Atom();
    this->cdrPtr = (Expression *) new Atom();
    this->expType = NODE_TYPE;
}

Node::Node(Atom * a1, Atom * a2){
    this->carPtr = (Expression *) a1;
    this-> cdrPtr = (Expression *) a2;
    this->expType = NODE_TYPE;
}
Node::Node(Atom * a1, Node * n2){
    this->carPtr = (Expression *) a1;
    this->cdrPtr = (Expression *) n2;
    this->expType = NODE_TYPE;
}
Node::Node(Node * n1, Atom * a2){
    this->carPtr = (Expression *) n1;
    this->cdrPtr = (Expression *) a2;
    this->expType = NODE_TYPE;
}
Node::Node(Node * n1, Node * n2){
    this->carPtr = (Expression *) n1;
    this->cdrPtr = (Expression *) n2;
    this->expType = NODE_TYPE;
}

Expression * Node::car() {
    return this->carPtr;
}
Expression * Node::cdr() {
    return this->cdrPtr;
}

Expression * Node::copy(){
    return Node::cons(this->car()->copy(), this->cdr()->copy());
}

Node * Node::cons(Expression * car, Expression * cdr) {
    Node * newNode;
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

Expression * Node::pop(Node** headPtr) { //sketchy
    Node * head = *headPtr;
    *headPtr = (Node *) head->cdr(); //May not work if cdr is atom*
    head->cdrPtr = new Atom();
    return head->car();
}

Expression * Node::peek() {
    return this;
}

void Node::rplaca(Expression* c){
    this->carPtr = c;
}

void Node::printRecur(){
    if(this->carPtr->getExpType() == NODE_TYPE){
        printf("(");
    }
    this->carPtr->printRecur();
    if(this->carPtr->getExpType() == NODE_TYPE){
        printf(")");
    }
    this->cdrPtr->printRecur();
}

void Node::print(){
    this->printRecur();
    std::cout << std::endl;
}
