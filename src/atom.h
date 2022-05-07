#pragma once

#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string>
#include <iostream>
#include <fstream>

#include "expression.h"

#define INTEGER 0
#define STRING 1
#define BOOLEAN 2
#define NIL_TYPE 3
#define NIL_STR "nil"

/* Boolean enum to avoid conflicting constructors with integers
   since Atom(0) and Atom(false) would be the same */
enum Boolean {
    f = 0,
    t = 1
};

typedef struct AtomVal {
    int64_t integer;
    std::string string;
    Boolean boolean;
} AtomVal;

class Atom : public Expression {
    AtomVal * val;
    int type;

public:
    Atom();
    Atom(int64_t integer);
    Atom(std::string string);
    Atom(Boolean boolean);
    AtomVal * get_atom();
    int64_t get_atom_integer();
    std::string get_atom_string();
    bool get_atom_boolean();
    int getType();
    void print();
    void printRecur();

    Expression * car();
    Expression * cdr();
    Expression * copy();

    static void checkDataTypeEq(Atom* a1, Atom* a2, int t);
    static Atom * add (Atom * op1, Atom * op2);
    static Atom * sub (Atom * op1, Atom * op2);
    static Atom * mul (Atom * op1, Atom * op2);
    static Atom * div (Atom * op1, Atom * op2);
    static Atom * rem (Atom * op1, Atom * op2);
    static Atom * gt (Atom * op1, Atom * op2);
    static Atom * lt (Atom * op1, Atom * op2);
    static Atom * geq (Atom * op1, Atom * op2);
    static Atom * leq (Atom * op1, Atom * op2);
};




