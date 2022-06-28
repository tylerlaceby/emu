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
  /////////////////////////
  //  Evaluation Blocks  //
  /////////////////////////

  Value* evaluateLiteral (Node* node, EmuEnv* env);
  Value* evaluateList (ListExpr* list, EmuEnv* env);
  Value* evaluate (Node* node, EmuEnv* env);
  Value* evaluateSymbol (Symbol* node, EmuEnv* env);
  Value* evaluateBlock (ListExpr* list, EmuEnv* env);

  ////////////////////////////////
  //     Commands & Operations  //
  ////////////////////////////////
  
  // Perform a variable declaration with the default variable being Null. Throws if variable is already declared.
  Value* var_cmd (ListExpr* list, EmuEnv* env, bool isConstantExpr);
  // Perform a variable assignment. Will throw an error id the variable is undefined.s
  Value* set_cmd (ListExpr* list, EmuEnv* env);
public:

  Value* eval (Node* ast);

};

#endif