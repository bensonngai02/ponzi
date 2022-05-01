#pragma once

#include "atom.h"
#include "node.h"
#include "expression.h"

class SECD {
    public:
        SECD();
        int i;
        std::string inputSStr;
        std::string* input;
        Node * stack;
        Node * environment;
        // need to read in control 
        Node * control;
        Node * dump;
    void execute(Node * inst_node);
    static void printStack(Node * node);
    void peekStackExpType(Node * stack, int expType);

};