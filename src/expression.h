#pragma once

class Expression {
public: 
    int expType;
    int getType();
    virtual void print() = 0;
    virtual bool eq(Expression * op) = 0;
};
