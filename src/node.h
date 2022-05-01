#pragma once
#include "expression.h"
#include "atom.h"

#define NODE_TYPE 1
#define NIL_TYPE 2

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

    bool eq(Expression * op);

    bool member(Node * target, Node * list);
    int position(Node * target, Node * list);
    Node * location(Node * target, Node * list);


    void print();

    static Node * cons(Expression * car, Expression * cdr);
    static void push(Node** headPtr, Expression * expression);
    static Expression * pop(Node** headPtr);
    Expression * peek();
};

    
