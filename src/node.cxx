#include "expression.cxx"

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

    Node * cons(Expression  car, Expression * cdr) {
        Node * newNode = new Node(car, cdr);
        return newNode;
    }

    int position(Node * target, Node * list) {
        bool type_eq = target->carPtr->getType() == list->carPtr->getType();
        bool val_eq = target->car == list->car();
        if (type_eq && val_eq) {
            return 0;
        }
        return 1 + position(target, list->cdr());
    }

    void print(){
        printf("(");
        this->carPtr->print();
        this->cdrPtr->print();
        printf(")");
    }

};