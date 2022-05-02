#pragma once

#define ATOM_TYPE 0
#define NODE_TYPE 1
#define NIL_TYPE 2

#define INTEGER 0
#define STRING 1
#define BOOLEAN 2

#include <iostream>

class Expression {
public: 
    int expType;
    int getExpType() {
        return expType;
    };
    static bool eq(Expression * op1, Expression * op2);
    virtual void print() = 0;
    virtual void printRecur() = 0;
    virtual Expression * car() = 0;
    virtual Expression * cdr() = 0;
    virtual Expression * copy() = 0;
};
