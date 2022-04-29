#pragma once

#include "atom.cxx"

typedef struct Node {
    struct Node * next;
    Atom * atom;
} Node;
