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

typedef union AtomVal {
    uint64_t integer;
    char * string;
    bool boolean;
} AtomVal;
typedef struct Atom {
    bool is_integer;
    bool is_string;
    bool is_boolean;
    AtomVal val;
} Atom;

