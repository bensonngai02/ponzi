#pragma once

#define ATOM_TYPE 4
#define NODE_TYPE 5

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

    Expression * cadr();
    Expression * cddr();
    Expression * cdar();
    Expression * caar();
    Expression * caddr();
    Expression * cadddr();
};
