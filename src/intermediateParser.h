#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdbool.h>
#include <ctype.h>

#include <optional>
#include <map>
#include <set>
#include <stack>
#include <string>

class IntermediateParser {
    void skip(std::string * str, int * index);
    std::string consumeIdentifier(std::string * str, int * index);
    std::string peek(std::string * str, int * index);
    Expression * consume(std:: string * str, int * index);
    std::string * createInterpretedString();
    int main();

};