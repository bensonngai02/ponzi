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

#include "expression.h"

#define INTEGER 0
#define STRING 1
#define BOOLEAN 2
#define ATOM_TYPE 0
#define NIL "nil"


enum Boolean {
    f = 0,
    t = 1
};

typedef struct AtomVal {
    uint64_t integer;
    std::string string;
    Boolean boolean;
} AtomVal;

class Atom : public Expression {
    bool is_integer;
    bool is_string;
    bool is_boolean;
    AtomVal * val;

public:
    Atom();
    Atom(uint64_t integer);
    Atom(std::string string);
    Atom(Boolean boolean);
    AtomVal * get_atom();
    uint64_t get_atom_integer();
    std::string get_atom_string();
    bool get_atom_boolean();
    int getType();
    Atom * add (Atom * op);
    Atom * sub (Atom * op);
    Atom * mul (Atom * op);
    Atom * div (Atom * op);
    Atom * rem (Atom * op);
    bool eq (Expression * op);
    bool geq (Atom * op);
    void print();

};
