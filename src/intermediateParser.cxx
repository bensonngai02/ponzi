#include "node.h"
#include "atom.h"
#include "expression.h"
#include "intermediateParser.h"


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

    if (isalnum((*str)[*index] ))
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
        Atom * new_atom = new Atom(temp);
        Expression* cdr = consume(str, index);
        if(cdr->expType == NIL_TYPE)
            return new Node(new_atom, new Atom());
        return new Node(new_atom,  (Node*) cdr);
    }
}

// (6 ())

// returns pointer to beginning of instruction s-expression to the control register
Expression * getControlPtr(std::string * string, int * index){
    return consume(string, index);
}

std::string createInterpretedString(){
    return "(LDC 6 LDC 3 ADD STOP)";
}

std::string createInterpretedString3(){
    std::ifstream t("instructions.txt");
    std::stringstream buffer;
    buffer << t.rdbuf();
    std::string* ret = new std::string(buffer.str());
    return *ret;
}

std::string createInterpretedString2(){
    std::ifstream t("instructions.txt");
    std::string str;

    t.seekg(0, std::ios::end);   
    str.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(t)),
                std::istreambuf_iterator<char>());
    return str;
}

