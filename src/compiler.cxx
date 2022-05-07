#include "compiler.h"
#include "parser.h"
#include "secd.h"

#define INPUT_ARG 1

Compiler::Compiler(std::string inputFile){
    std::string c = createInterpretedString(inputFile);
    int i = 0;
    code = consume(&c, &i);
    std::cout << "Parsed Scheme: ";
    code->print();
}

// selects list of variables from a passed in expression
Expression * Compiler::vars(Expression * d){
    if (d->getExpType() == NIL_TYPE) {
        return new Atom();
    }
    return Node::cons(d->caar(), vars(d->cdr())); 
}

// selects list of expressions from a passed in expression
Expression * Compiler::exprs(Expression * d) {
    if (d->getExpType() == NIL_TYPE) {
        return new Atom();
    }
    return Node::cons(d->cdar(), exprs(d->cdr()));
}

// compiles list of expressions (instructions from codelist + variables from namelist) and 
// generates code accordingly, recursively
Expression * Compiler::complis(Expression * expressions, Expression * namelist, Expression * codelist) {
    if (expressions->getExpType() == NIL_TYPE || Node::eq(expressions, new Node())) {
        return Node::cons(new Atom("NIL"), codelist);
    }
    return complis(expressions->cdr(), namelist, comp(expressions->car(), namelist, Node::cons(new Atom("CONS"), codelist)));
}

// Handles binary operations (add, sub, mul, div, rem) & boolean ops (geq, gt, leq, lt, eq)
// creates new code list in PONZI format based on prefix notation & operands of .scheme file
Expression * Compiler::binaryOp(Expression * expressions, Expression * namelist, Expression * codelist, std::string operation){
    Expression * newCodeList = comp(expressions->caddr(), namelist, Node::cons(new Atom(operation), codelist));
    return comp(expressions->cadr(), namelist, newCodeList);
}

// main compiler function
// replaces various scheme keywords with combination(s) of instructions part of PONZI ISA to create executable for SECD
Expression * Compiler::comp(Expression * expressions, Expression * namelist, Expression * codelist) {
    if (expressions->getExpType() != NODE_TYPE) {
        return Node::cons(new Atom("LD"), Node::cons(SECD::location((Node*) expressions, (Node*) namelist), codelist));
    }
    std::string checkStr = ((Atom *) expressions->car())->getAtomString();

    // QUOTE - needed in front of integer values in .scheme files for ease in parsing
    if (checkStr == "QUOTE") {
        return Node::cons(new Atom("LDC"), Node::cons(expressions->cadr(), codelist));
    }
    else if (checkStr == "ADD") {
        return binaryOp(expressions, namelist, codelist, "ADD");
    }
    else if (checkStr == "SUB") {
        return binaryOp(expressions, namelist, codelist, "SUB");
    }
    else if (checkStr == "MUL") {
        return binaryOp(expressions, namelist, codelist, "MUL");
    }
    else if (checkStr == "DIV") {
        return binaryOp(expressions, namelist, codelist, "DIV");
    }
    else if (checkStr == "REM") {
        return binaryOp(expressions, namelist, codelist, "REM");
    }
    else if (checkStr == "EQ") {
        return binaryOp(expressions, namelist, codelist, "EQ");
    }
    else if (checkStr == "LEQ") {
        return binaryOp(expressions, namelist, codelist, "LEQ");
    }
    else if (checkStr == "GEQ") {
        return binaryOp(expressions, namelist, codelist, "GEQ");
    }
    else if (checkStr == "LT") {
        return binaryOp(expressions, namelist, codelist, "LT");
    }
    else if ( checkStr == "CAR") {
        return comp(expressions->cadr(), namelist, Node::cons(new Atom("CAR"), codelist));
    }
    else if ( checkStr == "CDR") {
        return comp(expressions->cadr(), namelist, Node::cons(new Atom("CDR"), codelist));
    }
    else if ( checkStr == "ATOM") {
        return comp(expressions->cadr(), namelist, Node::cons(new Atom("ATOM"), codelist));
    }
    else if ( checkStr == "CONS") {
        Expression * newCodeList = comp(expressions->cadr(), namelist, Node::cons(new Atom("CONS"), codelist));
        return comp(expressions->caddr(), namelist, newCodeList);
    }
    else if (checkStr == "IF") {
        Expression * thenPt = comp(expressions->caddr(), namelist, Node::cons(new Atom("JOIN"), new Atom()) );
        Expression * elsePt = comp(expressions->cadddr(), namelist, Node::cons(new Atom("JOIN"), new Atom()) );
        return comp(expressions->cadr(), namelist, Node::cons(new Atom("SEL"), Node::cons(thenPt, Node::cons(elsePt, codelist))));
    }
    // LAMBDA - necessary to declare function, and apply if desired with passed in arguments. 
    else if (checkStr == "LAMBDA") {
        Expression * body = comp(expressions->caddr(), Node::cons(expressions->cadr(), namelist), Node::cons(new Atom("RET"), new Atom()) );
        return Node::cons(new Atom("LDF"), Node::cons(body, codelist));
    }
    // LET - keyword to declare function. However, not needed if lambda function is directly called.
    else if (checkStr == "LET") {
        Expression * m = Node::cons(vars(expressions->cddr()), namelist);
        Expression * args = exprs(expressions->cddr());
        Expression * body = comp(expressions->cadr(), m, Node::cons(new Atom("RET"), new Atom()) );
        return complis(args, namelist, Node::cons(new Atom("LDF"), Node::cons(body, Node::cons(new Atom("AP"), codelist))));
    }
    // LETREC - keyword to declare recursive function
    else if (checkStr == "LETREC") {
        Expression * m = Node::cons(vars(expressions->cddr()), namelist);
        Expression * args = exprs(expressions->cddr());
        Expression * body = comp(expressions->cadr(), m, Node::cons(new Atom("RET"), new Atom()));
        return Node::cons(new Atom("DUM"), complis(args, m, Node::cons(new Atom("LDF"), Node::cons(body, Node::cons(new Atom("RAP"), codelist)))));
    }
    else if (checkStr == "WRITE") {
        Expression * m = new Atom("WRITE");
        return Node:: cons(m, codelist);
    }   
    else if (checkStr == "FINN") {
        Expression * m = new Atom("FINN");
        return Node::cons(m, codelist);
    }
    else if (checkStr == "LEO") {
        Expression * m = new Atom("LEO");
        return Node::cons(m, codelist);
    }
    else if (checkStr == "NIKITA") {
        Expression * m = new Atom("NIKITA");
        return Node::cons(m, codelist);
    }
    else if (checkStr == "MICHAEL") {
        Expression * m = new Atom("MICHAEL");
        return Node::cons(m, codelist);
    }
    else {
        std::cout << "Expressions: " << checkStr << " "; expressions->print();
        return complis(expressions->cdr(), namelist, comp(expressions->car(), namelist, Node::cons(new Atom("AP"), codelist)));
    }
}

int main(int argc, char** argv){
    std::string input(argv[INPUT_ARG]);
    Compiler* compile = new Compiler(input);
    std::cout << std::endl;
    Expression* result = Compiler::comp(compile->code, new Atom(), new Atom("STOP"));
    std::cout << "Compiled Scheme into PONZI: ";
    result->print();
    std::cout << std::endl;
    SECD machine((Node* )result);
    while(true){
        machine.print();
        machine.execute();
    }
}
