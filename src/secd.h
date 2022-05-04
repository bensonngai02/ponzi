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
    void print(std::string fileName);
    void peekStackExpType(Node * stack, int expType);
    void mathOp(std::string operation);
    void boolOp(std::string operation);
    static void println(Node * string);
    void output(std::string fileName);

    static bool member(Node * target, Node * list);
    static int position(Node * target, Node * list);
    static Node * location(Node * target, Node * list);
};