#include "node.h"
#include "atom.h"
#include "expression.h"
#include "intermediateParser.h"

#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

void skip(std::string* str, int* index)
{
    while(isspace((*str)[*index])) {
        *index += 1;
    }
}

std::string consumeIdentifier(std::string* str, int* index)
{
    skip(str, index);
    std::string newString = "";

    if (isalpha((*str)[*index] ))
    {
        do
        {
            newString += (*str)[*index];
            *index += 1;
        } while (isalnum((*str)[*index]));
        return newString;
    }
    else
    {
        return "";
    }
}

std::string peek(std::string* str, int* index){
    int start = *index;
    std::string ret = consumeIdentifier(str, index);
    *index = start;
    return ret;
}

Expression * consume(std::string* str, int* index) {
    if (peek(str, index) == ")") {
        consumeIdentifier(str, index);
        return new Atom();  // return nil type
    }
    else if (peek(str, index) == "(") {
        while (peek(str, index) != ")") {
            consume(str, index);
        }
    }
    else {
        Atom * new_atom = new Atom(consumeIdentifier(str, index));
        Expression* cdr = consume(str, index);
        if(cdr->expType == NIL_TYPE)
            return new Node(new_atom, new Atom());
        return new Node(new_atom, (Node*) cdr);
    }
}

// (6 ())

// returns pointer to beginning of instruction s-expression to the control register
Expression * getControlPtr(std::string * string, int * index){
    return consume(string, index);
}

std::string* createInterpretedString(){
    std::ifstream t("instructions.txt");
    std::stringstream buffer;
    buffer << t.rdbuf();
    std::string* ret = new std::string(buffer.str());
    return ret;
}

int main(){
    std::string* str = createInterpretedString();
    std::cout << *str << std::endl;
}