#pragma once

#include "node.h"


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
    void execute();
    static void printStack(Node * node);
    void peekStackExpType(Node * stack, int expType);
    void mathOp(std::string operation);
    void boolOp(std::string operation);

};