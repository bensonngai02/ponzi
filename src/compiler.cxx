#include "compiler.h"

Expression * Node::vars(Node * d){
    if (d->getExpType() == NIL_TYPE) {
        return new Atom();
    }
    else {
        return cons(d->caar(), vars(d->cdr()));
    }
}

Expression * Node::exprs(Node * d) {
    if (d->getExpType() == NIL_TYPE) {
        return new Atom();
    }
    else {
        return cons(d->cdar(), exprs(d->cdr()));
    }
}

Expression * Compiler::complis(Expression * expressions, Expression * namelist, Expression * codelist) {
    if (namelist->getExpType() == NIL_TYPE) {
        return cons(new Atom("LDC"), cons(new Atom(), codelist));
    }
    else {
        complis(expressions->cdr(), namelist, comp(e->car(), namelist, cons(new Atom("CONS"), codelist)));
    }
}

Expression * Compiler::comp(Expression * expressions, Expression * namelist, Expression codelist) {
    if (expressions->getExpType() == ATOM_TYPE) {
        return cons(new Atom("LD"), cons())
    }
    else if (((Atom *) expressions->car())->get_atom_string() == "QUOTE") {
        return cons(new Atom("LDC"), cons(e->cadr(), codelist));
    }
    else if (((Atom *) expressions->car())->get_atom_string() == "ADD") {
        Expression * newCodeList = comp(e->caddr(), namelist, cons(new Atom("ADD"), codelist));
        return comp(expressions->cadr(), namelist, newCodeList);
    }
    else if (((Atom *) expressions->car())->get_atom_string == "CONS") {
        Expression * newCodeList = comp(e->cadr(), namelist, cons(new Atom("CONS"), codelist));
        return comp(e->caddr(), namelist, newCodeList);
    }
    else if (((Atom *) expressions->car())->get_atom_string == "IF") {
        Expression * thenPt = comp(e->caddr(), namelist, new Atom("JOIN"));
        Expression * elsePt = comp(e->cadddr(), namelist, new Atom("JOIN"));
        return comp(e->cadr(), namelist, cons(new Atom("SEL"), cons(thenPt, cons(elsePt, codelist))));
    }
    else if (((Atom *) expressions->car())->get_atom_string == "LAMBDA") {
        Expression * body = comp(e->caddr(), cons(e->cadr(), namelist), new Atom("RET"));
        return cons(new Atom("LDF"), cons(body, codelist));
    }
    else if (((Atom *) expressions->car())->get_atom_string == "LET") {
        Expression * m = cons(vars(e->cddr()), namelist);
        Expression * args = exprs(e->cddr());
        Expression * body = comp(e->cadr(), m, new Atom("RET"));
        return complis(args, n, cons(new Atom("LDF"), cons(body, cons(new Atom("AP"), codelist))));
    }
    else if (((Atom *) expressions->car())->get_atom_string == "LETREC") {
        Expression * m = cons(vars(e->cddr()), namelist);
        Expression * args = exprs(e->cddr());
        Expression * body = comp(e->cadr(), m, new Atom("RTN"));
        return cons(new Atom("DUM"). complis(args, m, cons(new Atom("LDF"), cons(body, cons(new Atom("RAP"), codelist)))));
    }
    else {
        return complis(e->cdr(), namelist, comp(e->car(), namelist, cons(new Atom("AP"), codelist)));
    }
}

