#include "atom.h"

class Atom {
    public:
        bool is_integer;
        bool is_string;
        bool is_boolean;
        AtomVal * val;
    
    Atom(uint64_t integer) {
        this->is_integer = true;
        this->val->integer = integer;
    }

    Atom(char * string) {
        this->is_string = true;
        this->val->string = string;
    }

    Atom(bool boolean) {
        this->is_boolean = true;
        this->val->boolean = boolean;
    }

    uint64_t get_atom_integer() {
        return this->val->integer;
    } 

    char * get_atom_string() {
        return this->val->string;
    }

    bool get_atom_boolean() {
        return this->val->boolean;
    }
};






