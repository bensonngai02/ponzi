#include "parser.h"

using namespace std;


struct Compiler
{
    char *program;
    char *current;
};

set<string> variables;

// forward declaration needed for function call in e1
uint64_t expression(bool effects, struct Compiler *compiler);
bool statement(bool effects, struct Compiler *compiler);

// global flag controls issue of returning 0 to else-if statement in function e1 (line 144)
bool literalFlag = true;

[[noreturn]]
void fail(struct Compiler *compiler)
{
    printf("current letter: %c\n", compiler->current[0]);
    printf("failed at offset %ld\n", (compiler->current - compiler->program));
    printf("%s\n", compiler->current); // print current char in string
    exit(1);
}

void endOrFail(struct Compiler *compiler)
{
    while (isspace(*(compiler->current)))
    {
        compiler->current += 1;
    }
    if (*(compiler->current) != 0)
        /* If current is not at end of string, fail */
        fail(compiler);
}

void skip(struct Compiler *compiler)
{
    while (isspace(*(compiler->current)))
    {
        compiler->current += 1;
    }
}

/* peeks at the next character */
bool peek(const char * str, struct Compiler *compiler) {
    skip(compiler);
 
    size_t i = 0; // size_t is an unsigned (>0) integer type of at least 2 bytes to represent size of object
    while (true)
    {
        char const expected = str[i];
        char const found = (compiler->current)[i];
        if (expected == 0)
        {
            /* survived to the end of the expected string */
            return true;
        }
        if (expected != found)
        {
            return false;
        }
        // assertion: found != 0
        // moves to the next character to consume
        i += 1;
    }
}

/* consumes the next available token in the given string input */
bool consume(const char *str, struct Compiler *compiler)
{
    skip(compiler);

    size_t i = 0; // size_t is an unsigned (>0) integer type of at least 2 bytes to represent size of object
    while (true)
    {
        char const expected = str[i];
        char const found = (compiler->current)[i];
        if (expected == 0)
        {
            /* survived to the end of the expected string */
            (compiler->current) += i;
            return true;
        }
        if (expected != found)
        {
            return false;
        }
        // assertion: found != 0
        // moves to the next character to consume
        i += 1;
    }
}

void consumeOrFail(const char *str, struct Compiler *compiler)
{
    if (!consume(str, compiler))
    {
        fail(compiler);
    }
}

/* returns string identifier that was consumed */
std::optional<string> consumeIdentifier(struct Compiler *compiler)
{
    skip(compiler);
    string newString = "";

    if (isalpha(*(compiler->current)))
    {
        do
        {
            newString += *(compiler->current);
            (compiler->current) += 1;
        } while (isalnum(*(compiler->current)));

        return newString;
    }
    else
    {
        return {};
    }
}

/* returns integer token found in expression from input */
uint64_t consumeLiteral(struct Compiler *compiler)
{
    skip(compiler);
    literalFlag = true;

    if (isdigit(*(compiler->current)))
    {
        uint64_t v = 0;
        do
        {
            v = 10 * v + (*(compiler->current) - '0');
            (compiler->current) += 1;
        } while (isdigit(*(compiler->current)));
        return v;
    }
    else
    {
        literalFlag = false;
        return false;
    }
}

void eatFunction(struct Compiler * compiler, bool effects) {
    // ignores the code in the function block
    if (consume("{", compiler)) {
        while (!consume("}", compiler)) {
            statement(effects, compiler);
        }
    }
}

uint64_t e1(bool effects, struct Compiler *compiler)
{
    uint64_t literal;
    uint64_t v = 0;
    int localFunctionCount;
    
    if (consume("fun", compiler)) {
        statement(effects, compiler);
    }
    
    else if (auto identifier = consumeIdentifier(compiler))
    {
        variables.insert({identifier.value()});
        
        if (compiler->current[0] == '('){
            
            consume("(", compiler);
            expression(effects, compiler);
            consume(")", compiler);

        }

        else {

            return v;
        }
        
    }
    else if ((literal = consumeLiteral(compiler)) || literalFlag)
    {
        /* MOVK used to account for large immediate numbers. 
           Loads in 16 bits per time */

        return literal;
    }
    else if (consume("(", compiler))
    {
        expression(effects, compiler);
        while (consume(")", compiler));
        return 0;
    }
    else
    {
        fail(compiler);
    }
    return false;
}

// ++ -- unary+ unary- ... (Right)
uint64_t e2(bool effects, struct Compiler *compiler)
{
    return (e1(effects, compiler));
}

// * / % (Left)
uint64_t e3(bool effects, struct Compiler *compiler)
{
    uint64_t v = e2(effects, compiler);

    while (true)
    {
        if (consume("*", compiler))
        {
            e2(effects, compiler);

        }
        else if (consume("/", compiler))
        {
            e2(effects, compiler);

        }
        else
        {
            return v;
        }
    }
}

// (Left) + -
uint64_t e4(bool effects, struct Compiler *compiler)
{
    uint64_t v = e3(effects, compiler);

    while (true)
    {
        if (consume("+", compiler))
        {
            e3(effects, compiler);


        }
        else if (consume("-", compiler))
        {
            e3(effects, compiler);

        }
        else
        {
            return v;
        }
    }
}

// << >>
uint64_t e5(bool effects, struct Compiler *compiler)
{
    return e4(effects, compiler);
}

// < <= > >=
uint64_t e6(bool effects, struct Compiler *compiler)
{
    uint64_t v = e5(effects, compiler);

    while (true)
    {
        if (consume("<=", compiler))
        {
            e5(effects, compiler);

        } 
        else if (consume(">=", compiler))
        {
            e5(effects, compiler);

        } 
        else if (consume("<", compiler))
        {
            e5(effects, compiler);

        } 
        else if (consume(">", compiler))
        {
            e5(effects, compiler);

        } 
        else {
            return v;
        }
    }
}

// == !=
uint64_t e7(bool effects, struct Compiler *compiler)
{
    uint64_t v = e6(effects,compiler);

    while (true)
    {
        if (consume("==", compiler)) {
            e6(effects, compiler);

        }
        else if (consume("!=", compiler)) {
            e6(effects, compiler);

        }
        else {
            return v;
        }
    }
}

// (left) &
uint64_t e8(bool effects, struct Compiler *compiler)
{
    uint64_t v = e7(effects, compiler);

    while (true) {
        if (!peek("&&", compiler) && consume("&", compiler)) {
            e7(effects, compiler);
            
        } else {
            return v;
        }
    }
}

// ^
uint64_t e9(bool effects, struct Compiler *compiler)
{
    return e8(effects, compiler);
}

// |
uint64_t e10(bool effects, struct Compiler *compiler)
{
    return e9(effects, compiler);
}

// &&
uint64_t e11(bool effects, struct Compiler *compiler)
{
    uint64_t v = e10(effects, compiler);

    while (true) {
        if (consume("&&", compiler)) {
           

            e10(effects, compiler);

            return 0;
            
        }
        else {
            return v;
        }
    }
}

// ||
uint64_t e12(bool effects, struct Compiler *compiler)
{
    uint64_t v = e11(effects, compiler);

    while (true) {
        
        if (consume("||", compiler)) {
            
            e11(effects, compiler);

            return 0;
        }
        else {
            return v;
        }
    }
}

// (right with special treatment for middle expression) ?:
uint64_t e13(bool effects, struct Compiler *compiler)
{
    return e12(effects, compiler);
}

// = += -= ...
uint64_t e14(bool effects, struct Compiler *compiler)
{
    return e13(effects, compiler);
}


uint64_t expression(bool effects, struct Compiler *compiler)
{
    return e14(effects, compiler);
}

bool statement(bool effects, struct Compiler *compiler)
{

    if (consume("if", compiler)) {
        
        expression(effects, compiler);
        
        statement(effects, compiler);
       
        if (consume("else", compiler)) {
            statement(effects, compiler);
        }

        return true;
    }
 
    /* Reads the entire block of code */
    else if (consume("{", compiler)) {
        while (!consume("}", compiler)) {
            statement(effects, compiler);
        }
    }

    else if (consume("return", compiler)) {
        
        expression(effects, compiler);

        return true;
    }
    else if (consume("print", compiler))
    {
        // print ...
        expression(effects, compiler);

        return true;
    }
    else if (auto identifier = consumeIdentifier(compiler))
    {
        variables.insert({identifier.value()});
        // x = ...
        if (consume("=", compiler))
        {
            expression(effects, compiler);
        

            return true;
        }
        else
            fail(compiler);
    }
    return false;
}


void statements(bool effects, struct Compiler *compiler)
{
    while (statement(effects, compiler));
}

void run(struct Compiler *compiler)
{
    statements(true, compiler);
    endOrFail(compiler);
}


int main(int argc, char** argv) {
    if (argc != 2) {
        printf("usage: %s <fun file name>\n",argv[0]);
        exit(-1);
    }

    // open the file
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        exit(1);
    }

    // determine its size (std::filesystem::get_size?)
    struct stat file_stats;
    int rc = fstat(fd, &file_stats);
    if (rc != 0)
    {
        perror("fstat");
        exit(1);
    }

    // map the file in my address space - mmap returns pointer to a block of memory
    char *prog = (char *)mmap(
        0,
        file_stats.st_size,
        PROT_READ,
        MAP_PRIVATE,
        fd,
        0);
    if (prog == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    run(compiler);
   
    return 0;
}