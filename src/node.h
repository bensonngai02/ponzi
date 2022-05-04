#pragma once
#include "expression.h"
#include "atom.h"

class Node : public Expression {
    Expression * carPtr;
    Expression * cdrPtr;

public:
    Node();
    Node(Atom * a1, Atom * a2);
    Node(Atom * a1, Node * n2);
    Node(Node * n1, Atom * a2);
    Node(Node * n1, Node * n2);
    Expression * car();
    Expression * cdr();
    Expression * copy();
    void rplaca(Expression * c);
    void print();
    void printRecur();
    
    static Node * cons(Expression * car, Expression * cdr);
    static void push(Node** headPtr, Expression * expression);
    static Expression * pop(Node** headPtr);
    Expression * peek();

};

    
