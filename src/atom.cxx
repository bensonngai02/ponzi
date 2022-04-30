#include "atom.h"
#include "main.cxx"
#include "expression.cxx"

#include <string>


Atom::Atom() {
    this->is_string = true;
    this->val->string = NIL;
    this->expType = ATOM_TYPE;
}

Atom::Atom(uint64_t integer) {
    this->is_integer = true;
    this->val->integer = integer;
    this->expType = ATOM_TYPE;
}

Atom::Atom(std::string string) {
    this->is_string = true;
    this->val->string = string;
    this->expType = ATOM_TYPE;
}

Atom::Atom(bool boolean) {
    this->is_boolean = true;
    this->val->boolean = boolean;
    this->expType = ATOM_TYPE;
}

AtomVal * Atom::get_atom() {
    return this->val;
}

uint64_t Atom::get_atom_integer() {
    return this->val->integer;
} 

std::string Atom::get_atom_string() {
    return this->val->string;
}

bool Atom::get_atom_boolean() {
    return this->val->boolean;
}

int Atom::getType(){
    return this->is_integer ? INTEGER : (this->is_string ? STRING : BOOLEAN);
}

Atom* Atom::add(Atom* op){
    if(this->getType() != INTEGER || op->getType() != INTEGER){
        printf("Trying to add two non integers");
        fail();
    }
    uint64_t sum = this->get_atom_integer() + op->get_atom_integer();
    Atom ret = new Atom(sum);
    return &ret;
}

Atom* Atom::sub(Atom* op){
    if(this->getType() != INTEGER || op->getType() != INTEGER){
        printf("Trying to sub two non integers");
        fail();
    }
    uint64_t diff = this->get_atom_integer() - op->get_atom_integer();
    Atom ret = new Atom(diff);
    return &ret;
}

Atom* Atom::mul(Atom* op){
    if(this->getType() != INTEGER || op->getType() != INTEGER){
        printf("Trying to mul two non integers");
        fail();
    }
    uint64_t prod = this->get_atom_integer() * op->get_atom_integer();
    Atom ret = new Atom(prod);
    return &ret;
}

Atom* Atom::div(Atom* op){
    if(this->getType() != INTEGER || op->getType() != INTEGER){
        printf("Trying to sub two non integers");
        fail();
    }
    uint64_t div = this->get_atom_integer() / op->get_atom_integer();
    Atom ret = new Atom(div);
    return &ret;
}

Atom* Atom::rem(Atom* op){
    if(this->getType() != INTEGER || op->getType() != INTEGER){
        printf("Trying to sub two non integers");
        fail();
    }
    uint64_t rem = this->get_atom_integer() % op->get_atom_integer();
    Atom ret = new Atom(rem);
    return &ret;
}

bool Atom::eq(Expression* op){
    bool same_type = this->getType() == ((Atom *) op)->getType();
    bool intEq = this->get_atom_integer() == ((Atom *) op)->get_atom_integer();
    bool strEq = this->get_atom_string() == ((Atom *) op)->get_atom_string();
    bool boolEq = this->get_atom_boolean() == ((Atom *) op)->get_atom_boolean();
    bool valEq = intEq || strEq || boolEq;
    return same_type && valEq;
}

bool Atom::geq(Atom * op){
    bool is_integer = this->getType() == INTEGER;
    bool same_type = is_integer && this->getType() == op->getType();
    if (!same_type)
        fail();
    bool intEq = this->get_atom_integer() >= ((Atom *) op)->get_atom_integer();
    return intEq;
}

void Atom::print(){
    if(this->is_boolean){
        printf("%b ", this->get_atom_boolean());
    }
    else if(this->is_integer){
        printf("%d ", this->get_atom_integer());
    }
    else if(this->get_atom_string() != NIL){
        printf("%s ", this->get_atom_string());
    }
}





