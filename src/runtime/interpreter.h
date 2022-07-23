#ifndef EMU_RUNTIME
#define EMU_RUMTIME

#include "Environment.h"
#include "Values.h"
#include "../parser/ast.h"

class Interpreter {

private:
    Val* eval (Node* node, Env* env);
    Val* eval_literal (LiteralExpression* node, Env* env);

public:
    Val* execute (Node* node);
};


#endif