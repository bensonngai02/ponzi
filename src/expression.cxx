#include "expression.h"
#include "atom.h"
#include "node.h"

bool Expression::eq(Expression * op1, Expression * op2) {
    if (op1->getExpType() != op2->getExpType()) {
        return false;
    }
    // both items on stack are atoms
    if(op1->getExpType() == NIL_TYPE && op2->getExpType() == NIL_TYPE){
        return true;
    }
    if (op1->getExpType() == ATOM_TYPE && op2->getExpType() == ATOM_TYPE) {
        Atom * atomOp1 = (Atom *) op1;
        Atom * atomOp2 = (Atom *) op2;
        if (atomOp1->getType() != atomOp2->getType()) {
            std::cout << "Atoms are not the same datatype (int, string, bool) so they cannot be compared";
            return false;
        }

        int atomType = atomOp1->getType();

        if (atomType == INTEGER) {
            return atomOp1->get_atom_integer() == atomOp2->get_atom_integer();
        }
        else if (atomType == STRING) {
            return atomOp1->get_atom_string() == atomOp2->get_atom_string();
        }
        else if (atomType == BOOLEAN) {
            return atomOp1->get_atom_boolean() == atomOp2->get_atom_boolean();
        }
        
        return false;
    }
    // both items on stack are nodes
    else {
        Node * nodeOp1 = (Node *) op1;
        Node * nodeOp2 = (Node *) op2;

        if (nodeOp1->getExpType() != nodeOp2->getExpType()) {
            return false;
        }
        bool carPtrEq = Node::eq(nodeOp1->car(), nodeOp2->car()); 
        bool cdrPtrEq = Node::eq(nodeOp1->cdr(), nodeOp2->cdr());

        return carPtrEq && cdrPtrEq;
    }
}

Expression * Expression::cadr(){
    return this->cdr()->car();
}
Expression * Expression::cddr(){
    return this->cdr()->cdr();
}
Expression * Expression::cdar(){
    return this->car()->cdr();
}
Expression * Expression::caar(){
    return this->car()->car();
}
Expression * Expression::caddr(){
    return this->cdr()->cdr()->car();
}
Expression * Expression::cadddr(){
    return this->cdr()->cdr()->cdr()->car();
}