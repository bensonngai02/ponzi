#include "compiler.h"
#include "intermediateParser.h"

Compiler::Compiler(std::string inputFile){
    std::string c = createInterpretedString(inputFile);
    int i = 0;
    code = consume(&c, &i);
}

Expression * Compiler::vars(Expression * d){
    if (d->getExpType() == NIL_TYPE) {
        return new Atom();
    }
    return Node::cons(d->caar(), vars(d->cdr())); 
}

Expression * Compiler::exprs(Expression * d) {
    if (d->getExpType() == NIL_TYPE) {
        return new Atom();
    }
    return Node::cons(d->cdar(), exprs(d->cdr()));
}

Expression * Compiler::complis(Expression * expressions, Expression * namelist, Expression * codelist) {
    if (expressions->getExpType() == NIL_TYPE) {
        return Node::cons(new Atom("LDC"), Node::cons(new Atom(), codelist));
    }
    return complis(expressions->cdr(), namelist, comp(expressions->car(), namelist, Node::cons(new Atom("CONS"), codelist)));
}

Expression * Compiler::comp(Expression * expressions, Expression * namelist, Expression * codelist) {
    std::cout << "Expression: "; expressions->print();
    std::cout << "Name List: ";namelist->print();
    std::cout << "Code List: ";codelist->print();
    if (expressions->getExpType() != NODE_TYPE) {
        return Node::cons(new Atom("LD"), Node::cons(Node::location((Node*) expressions, (Node*) namelist), codelist ));
    }
    else if (((Atom *) expressions->car())->get_atom_string() == "QUOTE") {
        return Node::cons(new Atom("LDC"), Node::cons(expressions->cadr(), codelist));
    }
    else if (((Atom *) expressions->car())->get_atom_string() == "ADD") {
        Expression * newCodeList = comp(expressions->caddr(), namelist, Node::cons(new Atom("ADD"), codelist));
        return comp(expressions->cadr(), namelist, newCodeList);
    }
    else if ( ((Atom *) expressions->car())->get_atom_string() == "CONS") {
        Expression * newCodeList = comp(expressions->cadr(), namelist, Node::cons(new Atom("CONS"), codelist));
        return comp(expressions->caddr(), namelist, newCodeList);
    }
    else if (((Atom *) expressions->car())->get_atom_string() == "IF") {
        Expression * thenPt = comp(expressions->caddr(), namelist, Node::cons(new Atom("JOIN"), new Atom()) );
        Expression * elsePt = comp(expressions->cadddr(), namelist, Node::cons(new Atom("JOIN"), new Atom()) );
        return comp(expressions->cadr(), namelist, Node::cons(new Atom("SEL"), Node::cons(thenPt, Node::cons(elsePt, codelist))));
    }
    else if (((Atom *) expressions->car())->get_atom_string() == "LAMBDA") {
        Expression * body = comp(expressions->caddr(), Node::cons(expressions->cadr(), namelist), Node::cons(new Atom("RET"), new Atom()) );
        return Node::cons(new Atom("LDF"), Node::cons(body, codelist));
    }
    else if (((Atom *) expressions->car())->get_atom_string() == "LET") {
        Expression * m = Node::cons(vars(expressions->cddr()), namelist);
        Expression * args = exprs(expressions->cddr());
        Expression * body = comp(expressions->cadr(), m, Node::cons(new Atom("RET"), new Atom()) );
        return complis(args, namelist, Node::cons(new Atom("LDF"), Node::cons(body, Node::cons(new Atom("AP"), codelist))));
    }
    else if (((Atom *) expressions->car())->get_atom_string() == "LETREC") {
        Expression * m = Node::cons(vars(expressions->cddr()), namelist);
        Expression * args = exprs(expressions->cddr());
        Expression * body = comp(expressions->cadr(), m, Node::cons(new Atom("RET"), new Atom()));
        return Node::cons(new Atom("DUM"), complis(args, m, Node::cons(new Atom("LDF"), Node::cons(body, Node::cons(new Atom("RAP"), codelist)))));
    }
    else {
        return complis(expressions->cdr(), namelist, comp(expressions->car(), namelist, Node::cons(new Atom("AP"), codelist)));
    }
}

int main(){
    Compiler* compile = new Compiler("src/lispcode.txt");
    Expression* result = Compiler::comp(compile->code, new Atom(), new Atom());
    result->print();
}
