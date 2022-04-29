#pragma once

typedef struct Node {
    struct Node * next;
    struct Atom * atom;
} Node;