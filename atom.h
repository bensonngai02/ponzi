#pragma once

#include <stdlib.h>

typedef union Atom {
    uint64_t integer;
    char * string;
    bool boolean;
} Atom;