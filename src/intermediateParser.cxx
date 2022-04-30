#include "intermediateParser.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

void skip(std::string* str)
{
    while (isspace(*(compiler->current)))
    {
        compiler->current += 1;
    }
}

void consumeExpression(){
    
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