#include "node.h"

Node::Node() {
    this->carPtr = (Expression *) new Atom();
    this->cdrPtr = (Expression *) new Atom();
    //this->expType = NODE_TYPE;
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

bool Node::member(Node * target, Node * list) {
    if (list->getExpType() == NIL_TYPE)
        return false;
    if (Node::eq(target, list->car()))
        return true;
    else 
        return member(target, (Node *) list->cdr());
}

int Node::position(Node * target, Node * list) {
    bool val_eq = Node::eq(target, list->car());
    return val_eq? 0 : 1 + position(target, (Node *) list->cdr());
}

Node * Node::location(Node * target, Node * list) {
    if(eq(new Atom(), list->car())){
            std::cout << "Reached end of list without finding locaiton";
            exit(1);
        }
    if (member(target, (Node *) list->car())) {
        int got_pos = position(target, (Node *) list->car());
        Atom * car_atom = new Atom(0);
        Atom * cdr_atom = new Atom(got_pos);
        return cons(car_atom, cdr_atom);
    }
    else {
        Node * z = (location(target, (Node *) list->cdr()));
        Atom * car_z = (Atom *) z->car();
        Atom * cdr_z = (Atom *) z->cdr();
        Atom * new_car = new Atom(car_z->get_atom_integer() + 1);
        Atom * new_cdr = new Atom(cdr_z->get_atom_integer());
        return cons(new_car, new_cdr);
    }
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


void Node::printRecur(){
    printf("(");
    this->carPtr->printRecur();
    this->cdrPtr->printRecur();
    printf(")");
}

void Node::print(){
    this->printRecur();
    std::cout <<std::endl;
}
