#ifndef EMU_TREE_WALKER_RUNTIME
#define EMU_TREE_WALKER_RUNTIME

#include "../emu.h"
#include "../parser/ast.h"
#include "../parser/parser.h"
#include "environment.h"
#include "values.h"
#include <map>

// DEFINE THE CLASS FOR THE INTERPRETER

class Interpreter {
private:

  Value* evaluateLiteral (Node* node, EmuEnv* env);
  Value* evaluateList (ListExpr* list, EmuEnv* env);
  Value* evaluate (Node* node, EmuEnv* env);
public:

  Value* eval (Node* ast);

};

#endif