#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdbool.h>
#include <ctype.h>

#include <optional>
#include <map>
#include <set>
#include <stack>
#include <string>
#include "string.h"

#include "expression.h"
#include "node.h"
#include "atom.h"

class Compiler {
    public:
        Compiler(std::string inputFile);
        Expression * code;
    static Expression * vars(Expression * d);
    static Expression * exprs(Expression * d);
    static Expression * complis(Expression * expressions, Expression * nodelist, Expression * controllist);
    static Expression * comp(Expression * expressions, Expression * namelist, Expression * codelist);
    static Expression * binaryOp(Expression * expressions, Expression * namelist, Expression * codelist, std::string s);
};