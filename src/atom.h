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

#define INTEGER 0
#define STRING 1
#define BOOLEAN 2
#define ATOM_TYPE 0
#define NIL "nil"

typedef union AtomVal {
    uint64_t integer;
    char * string;
    bool boolean;
} AtomVal;