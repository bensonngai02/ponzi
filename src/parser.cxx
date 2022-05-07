#include "node.h"
#include "atom.h"
#include "expression.h"
#include "parser.h"


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

    if (isalnum((*str)[*index]) || (*str)[*index] == '-')
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

// Consumes input to create an Expression (consisting of nodes + atoms)
Expression * consume(std::string* str, int* index) {
    skip(str, index);
    if ((*str)[*index] == ')' || *index == str->length()) {
        *index += 1;
        return new Atom();  // return nil type
    }
    else if ((*str)[*index] == '(') {
        *index += 1;
        Expression* car = consume(str, index);
        Expression* cdr = consume(str, index);
        Node n;
        return (Expression*) n.cons(car, cdr);
    }
    else {
        std::string temp = consumeIdentifier(str, index);
        if(temp.length() == 0){
            std::cout << "Found non alnum atom -- not allowed: " << temp << " " << *index << std::endl;
            exit(1);
        }
        Atom * new_atom;
        try{
            int64_t val = (int64_t) std::stoi(temp);
            new_atom  = new Atom(val);
        }catch(...){
            new_atom = new Atom(temp);
        }
        Expression* cdr = consume(str, index);
        if(cdr->expType == NIL_TYPE)
            return new Node(new_atom, new Atom());
        return new Node(new_atom,  (Node*) cdr);
    }
}

// returns pointer to beginning of instruction s-expression to the control register
Expression * getControlPtr(std::string * string, int * index){
    return consume(string, index);
}

// Parses the .scheme file to create an tokenized input that is fed into compiler
std::string createInterpretedString(std::string inputFile){
    std::ifstream t(inputFile);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

