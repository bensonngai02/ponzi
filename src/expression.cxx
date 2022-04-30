#include "expression.h"
#include "atom.h"

class Expression {
    public:
        int expType;
    
    int getType() {
        return expType;
    }
    virtual void print();


};




