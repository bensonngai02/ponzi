#include "expression.cxx"
#include "node.h"
#include "atom.cxx"

Atom * NIL_ATOM = new Atom();

class Node: public Expression {
    public:
        Expression * carPtr;
        Expression * cdrPtr;
    
    Node(Atom * a1, Atom * a2){
        this->carPtr = (Expression *) a1;
        this-> cdrPtr = a2;
        this->expType = NODE;
    }
    Node(Atom * a1, Node * n2){
        this->carPtr = a1;
        this->cdrPtr = n2;
        this->expType = NODE;
    }
    Node(Node * n1, Atom * a2){
        this->carPtr = n1;
        this->cdrPtr = a2;
        this->expType = NODE;
    }
    Node(Node * n1, Node * n2){
        this->carPtr = n1;
        this->cdrPtr = n2;
        this->expType = NODE;
    }

    Expression * car() {
        return this->carPtr;
    }

    Expression * cdr() {
        return this->cdrPtr;
    }

    Node * cons(Expression car, Expression * cdr) {
        Node * newNode = new Node(car, cdr);
        return newNode;
    }

    bool eq(Expression * op){
        if (this->getType != op->getType()) {
            return false;
        }
        bool carPtrEq = this->carPtr == ((Node *) op)->carPtr;
        bool cdrPtrEq = this->cdrPtr == ((Node *) op)->cdrPtr;
        return carPtrEq && cdrPtrEq;
    }

    bool member(Node * target, Node * namelist) {
        if (namelist->getType == NIL_TYPE)
            return false;
        if (target == namelist->car())
            return true;
        else 
            member(target, namelist->cdr());
    }

    int position(Node * target, Node * list) {
        bool type_eq = target->carPtr->getType() == list->carPtr->getType();
        bool val_eq = target->car == list->car();
        if (type_eq && val_eq) {
            return 0;
        }
        return 1 + position(target, list->cdr());
    }

    Node * location(Node * target, Node * list) {
        if (member(target, list)) {
            int got_pos = position(target, list);
            Atom * car_atom = new Atom(0);
            Atom * cdr_atom = new Atom(got_pos);
            return cons(zero_atom, cdr_atom);
        }
        else {
            int z = location(target, cdr(list));
            Atom * recur_car_atom = (Atom * (car(z))
            return cons(car(z)+1, cdr(z));
        }
    }

    void print(){
        printf("(");
        this->carPtr->print();
        this->cdrPtr->print();
        printf(")");
    }

};