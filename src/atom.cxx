#include "atom.h"
#include "expression.h"


/* Default constructor defaults to NIL type*/
Atom::Atom() {
    this->val = new AtomVal;
    this->type = NIL_TYPE;
    this->val->string = "nil";
    this->expType = NIL_TYPE;
}

Atom::Atom(int64_t integer) {
    this->val = new AtomVal;
    this->type = INTEGER;
    this->val->integer = integer;
    this->expType = ATOM_TYPE;
}

Atom::Atom(std::string string) {
    this->val = new AtomVal;
    this->type = STRING;
    this->val->string = string;
    this->expType = ATOM_TYPE;
}

Atom::Atom(Boolean boolean) {
    this->val = new AtomVal;
    this->type = BOOLEAN;
    this->val->boolean = boolean;
    this->expType = ATOM_TYPE;
}

AtomVal * Atom::get_atom() {
    return this->val;
}

int64_t Atom::get_atom_integer() {
    return this->val->integer;
} 

std::string Atom::get_atom_string() {
    return this->val->string;
}

bool Atom::get_atom_boolean() {
    return this->val->boolean;
}

int Atom::getType(){
    return type;
}

void Atom::checkDataTypeEq(Atom * atom1, Atom * atom2, int targetDataType) {
    if(atom1->getType() != INTEGER || atom2->getType() != targetDataType){
        printf("Trying to operate (+, -, *, /) on two non integers");
        exit(1);
    }
}

Atom* Atom::add(Atom * op1, Atom * op2){
    checkDataTypeEq(op1, op2, INTEGER);
    int64_t sum = op1->get_atom_integer() + op2->get_atom_integer();
    Atom * ret = new Atom(sum);
    return ret;
}

Atom* Atom::sub(Atom * op1, Atom * op2){
    checkDataTypeEq(op1, op2, INTEGER);
    int64_t diff = op1->get_atom_integer() - op2->get_atom_integer();
    Atom * ret = new Atom(diff);
    return ret;
}

Atom* Atom::mul(Atom * op1, Atom * op2){
    checkDataTypeEq(op1, op2, INTEGER);
    int64_t prod = op1->get_atom_integer() * op2->get_atom_integer();
    Atom * ret = new Atom(prod);
    return ret;
}

Atom* Atom::div(Atom * op1, Atom * op2){
    checkDataTypeEq(op1, op2, INTEGER);
    if (op2->get_atom_integer() == 0) {
        std::cout << "Error: Divide by zero";
        exit(1);
    }
    int64_t div = op1->get_atom_integer() / op2->get_atom_integer();
    Atom * ret = new Atom(div);
    return ret;
}

Atom* Atom::rem(Atom * op1, Atom * op2){
    checkDataTypeEq(op1, op2, INTEGER);
    int64_t rem = op1->get_atom_integer() % op2->get_atom_integer();
    Atom * ret = new Atom(rem);
    return ret;
}

/* Greater than */
Atom * Atom::gt(Atom * op1, Atom * op2){
    bool op1_is_integer = op1->getType() == INTEGER;
    bool op2_is_integer = op2->getType() == INTEGER;
    bool same_type = op1_is_integer && op2_is_integer && op1->getType() == op2->getType();
    if (!same_type)
        exit(1);
    bool val = ((Atom *) op1)->get_atom_integer() > ((Atom *) op2)->get_atom_integer();
    Boolean result = val ? t : f;
    return new Atom(result);
}

/* Less than */
Atom * Atom::lt(Atom * op1, Atom * op2){
    bool op1_is_integer = op1->getType() == INTEGER;
    bool op2_is_integer = op2->getType() == INTEGER;
    bool same_type = op1_is_integer && op2_is_integer && op1->getType() == op2->getType();
    if (!same_type)
        exit(1);
    bool val = ((Atom *) op1)->get_atom_integer() < ((Atom *) op2)->get_atom_integer();
    Boolean result = val ? t : f;
    return new Atom(result);
}

/* Greater than or equal to */
Atom * Atom::geq(Atom * op1, Atom * op2){
    bool op1_is_integer = op1->getType() == INTEGER;
    bool op2_is_integer = op2->getType() == INTEGER;
    bool same_type = op1_is_integer && op2_is_integer && op1->getType() == op2->getType();
    if (!same_type)
        exit(1);
    bool val = ((Atom *) op1)->get_atom_integer() >= ((Atom *) op2)->get_atom_integer();
    Boolean result = val ? t : f;
    return new Atom(result);
}

/* Less than or equal to */
Atom * Atom::leq(Atom * op1, Atom * op2){
    bool op1_is_integer = op1->getType() == INTEGER;
    bool op2_is_integer = op2->getType() == INTEGER;
    bool same_type = op1_is_integer && op2_is_integer && op1->getType() == op2->getType();
    if (!same_type)
        exit(1);
    bool val = ((Atom *) op1)->get_atom_integer() <= ((Atom *) op2)->get_atom_integer();
    Boolean result = val ? t : f;
    return new Atom(result);
}

/* Recursion-based print method */
void Atom::print(){
    this->printRecur();
    std::cout << std::endl;
}

void Atom::printRecur(){
    if(this->type == STRING){
        std::cout << this->get_atom_string() << " ";
    }
    else if(this->type == BOOLEAN){
        std::cout << this->get_atom_boolean() << " ";
    }
    else if(this->type == INTEGER){
        std::cout << this->get_atom_integer() << " ";
    }
    else if (this->type == NIL_TYPE) {
        std::cout << "NIL ";
    }
    else if( this-> type != NIL_TYPE){
        std::cout <<"Printing something unknown: " << this->type << " " << (this->type == STRING) << "\n";
        exit(1);
    }
}

/* Returns the first (head) part of a node */
Expression * Atom::car() {
    return this;
}

/* Returns the second (tail) part of a node */
Expression * Atom::cdr(){
    return new Atom();
}

Expression * Atom::copy(){
    if(this->getType() == NIL_TYPE){
        return new Atom();
    }
    if(this->type == BOOLEAN){
        Boolean b = this->get_atom_boolean() ? t : f;
        return new Atom(b);
    }
    if(this->type == INTEGER){
        return new Atom(this->get_atom_integer());
    }
    std::string * cpStr = new std::string(this->get_atom_string());
    return new Atom(*cpStr);
}





