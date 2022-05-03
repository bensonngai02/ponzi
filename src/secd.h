#pragma once

#include "node.h"
#include "string.h"


class SECD {
    public:
        SECD(std::string s);
        SECD(Node* con);
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
    static void println(Node * string);

};