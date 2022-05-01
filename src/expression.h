#pragma once

#define ATOM_TYPE 0
#define NODE_TYPE 1
#define NIL_TYPE 2

#define INTEGER 0
#define STRING 1
#define BOOLEAN 2

#include <iostream>
#include "atom.h"
#include "node.h"

class Expression {
public: 
    int expType;
    int getExpType() {
        return expType;
    };
    static bool eq(Expression * op1, Expression * op2) {
        if (op1->getExpType() != op2->getExpType()) {
            exit(1);
        }
        // both items on stack are atoms
        if (op1->getExpType() == ATOM_TYPE && op2->getExpType() == ATOM_TYPE) {
            Atom * atomOp1 = (Atom *) op1;
            Atom * atomOp2 = (Atom *) op2;
            if (atomOp1->getType() != atomOp2->getType()) {
                std::cout << "Atoms are not the same datatype (int, string, bool) so they cannot be compared";
                exit(1);
            }

            bool same_type = atomOp1->getType() == atomOp2->getType();
            bool intEq = atomOp1->get_atom_integer() == atomOp2->get_atom_integer();
            bool strEq = atomOp2->get_atom_string() == atomOp2->get_atom_string();
            bool boolEq = atomOp1->get_atom_boolean() == atomOp2->get_atom_boolean();
            bool valEq = intEq || strEq || boolEq;
            return same_type && valEq;
        }
        // both items on stack are nodes
        else {
            Node * nodeOp1 = (Node *) op1;
            Node * nodeOp2 = (Node *) op2;

            if (nodeOp1->getExpType() != nodeOp2->getExpType()) {
                return false;
            }
            bool carPtrEq = nodeOp1->car() == nodeOp2->car();
            bool cdrPtrEq = nodeOp1->cdr() == nodeOp2->cdr();

            return carPtrEq && cdrPtrEq;
        }
    };
    virtual void print() = 0;
    virtual Expression * car() = 0;
    virtual Expression * cdr() = 0;
};
