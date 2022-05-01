#pragma once

class Expression {
public: 
    int expType;
    int getExpType() {
        return expType;
    };
    virtual void print() = 0;
    virtual bool eq(Expression * op) = 0;
    virtual Expression * car() = 0;
    virtual Expression * cdr() = 0;
};
