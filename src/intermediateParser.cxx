#include "intermediateParser.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>


std::string* createInterpretedString(){
    std::ifstream t("instructions.txt");
    std::stringstream buffer;
    buffer << t.rdbuf();
    return &(buffer.str());
}

int main(){
    std::string* str = createInterpretedString();
    std::cout << *str << std::endl;
}