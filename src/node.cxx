#include "node.h"

Node::Node() {
    this->carPtr = (Expression *) new Atom();
    this->cdrPtr = (Expression *) new Atom();
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

bool Node::eq(Expression * op){
    if (this->getType() != op->getType()) {
        return false;
    }
    bool carPtrEq = this->carPtr == ((Node *) op)->carPtr;
    bool cdrPtrEq = this->cdrPtr == ((Node *) op)->cdrPtr;
    return carPtrEq && cdrPtrEq;
}

bool Node::member(Node * target, Node * list) {
    if (list->getType() == NIL_TYPE)
        return false;
    if (target == list->car())
        return true;
    else 
        member(target, (Node *) list->cdr());
}

int Node::position(Node * target, Node * list) {
    bool type_eq = target->carPtr->getType() == list->carPtr->getType();
    bool val_eq = target->car() == list->car();
    if (type_eq && val_eq) {
        return 0;
    }
    return 1 + position(target, (Node *) list->cdr());
}

Node * Node::location(Node * target, Node * list) {
    if (member(target, list)) {
        int got_pos = position(target, list);
        Atom * car_atom = new Atom(0);
        Atom * cdr_atom = new Atom(got_pos);
        return cons(car_atom, cdr_atom);
    }
    else {
        Node * z = (location(target, (Node *) list->cdr()));
        Atom * car_z = (Atom *) z->car();
        Atom * cdr_z = (Atom *) z->cdr();
        Expression * new_car = new Atom(car_z->get_atom_integer() + 1);
        Expression * new_cdr = new Atom(cdr_z->get_atom_integer());
        return cons(new_car, new_cdr);
    }
}

void Node::print(){
    printf("(");
    this->carPtr->print();
    this->cdrPtr->print();
    printf(")");
}
