#include "atom.h"
#include "main.cxx"
#include "expression.cxx"

class Atom : public Expression {
    public:
        bool is_integer;
        bool is_string;
        bool is_boolean;
        AtomVal * val;

    Atom() {
        this->is_string = true;
        this->val->string = NIL;
        this->expType = ATOM_TYPE;
    }
    
    Atom(uint64_t integer) {
        this->is_integer = true;
        this->val->integer = integer;
        this->expType = ATOM_TYPE;
    }

    Atom(char * string) {
        this->is_string = true;
        this->val->string = string;
        this->expType = ATOM_TYPE;
    }

    Atom(bool boolean) {
        this->is_boolean = true;
        this->val->boolean = boolean;
        this->expType = ATOM_TYPE;
    }

    AtomVal * get_atom() {
        return this->val;
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

    int getType(){
        return this->is_integer ? INTEGER : (this->is_string ? STRING : BOOLEAN);
    }

    Atom* add(Atom* op){
        if(this->getType() != INTEGER || op->getType() != INTEGER){
            printf("Trying to add two non integers");
            fail();
        }
        uint64_t sum = this->get_atom_integer() + op->get_atom_integer();
        Atom ret = new Atom(sum);
        return &ret;
    }

    Atom* sub(Atom* op){
        if(this->getType() != INTEGER || op->getType() != INTEGER){
            printf("Trying to sub two non integers");
            fail();
        }
        uint64_t diff = this->get_atom_integer() - op->get_atom_integer();
        Atom ret = new Atom(diff);
        return &ret;
    }

    Atom* mul(Atom* op){
        if(this->getType() != INTEGER || op->getType() != INTEGER){
            printf("Trying to mul two non integers");
            fail();
        }
        uint64_t prod = this->get_atom_integer() * op->get_atom_integer();
        Atom ret = new Atom(prod);
        return &ret;
    }

    Atom* div(Atom* op){
        if(this->getType() != INTEGER || op->getType() != INTEGER){
            printf("Trying to sub two non integers");
            fail();
        }
        uint64_t div = this->get_atom_integer() / op->get_atom_integer();
        Atom ret = new Atom(div);
        return &ret;
    }

    Atom* rem(Atom* op){
        if(this->getType() != INTEGER || op->getType() != INTEGER){
            printf("Trying to sub two non integers");
            fail();
        }
        uint64_t rem = this->get_atom_integer() % op->get_atom_integer();
        Atom ret = new Atom(rem);
        return &ret;
    }

    bool eq(Expression* op){
        bool same_type = this->getType() == op->getType();
        bool intEq = this->get_atom_integer() == ((Atom *) op)->get_atom_integer();
        bool strEq = this->get_atom_string() == ((Atom *) op)->get_atom_string();
        bool boolEq = this->get_atom_boolean() == ((Atom *) op)->get_atom_boolean();
        bool valEq = intEq || strEq || boolEq;
        return same_type && valEq;
    }

    Atom* geq(Atom* op){
        if(this->getType() != INTEGER || op->getType() != INTEGER){
            printf("Trying to sub two non integers");
            fail();
        }
        uint64_t rem = this->get_atom_integer() >= op->get_atom_integer();
        Atom ret = new Atom(rem);
        return &ret;
    }

    void print(){
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

};






