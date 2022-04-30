#pragma once

#include "atom.h"
#include "node.h"

class Expression {

public: 
    int expType;
    int getType();
    virtual void print();
    virtual bool eq(Expression * op);
};
