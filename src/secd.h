#pragma once

#include "atom.h"
#include "node.h"
#include "expression.h"

class SECD {
    public:
        SECD();

    void push(Expression * expression);
    Expression * pop();
    Expression * peek();


};