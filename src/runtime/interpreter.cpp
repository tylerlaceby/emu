#include "interpreter.h"
#include <vector>
#include <map>

Value* Interpreter::eval (Node* ast) {

  // define the global scope
  EmuEnv* env = CreateGlobalEnvironment();
  return evaluate(ast, env);
}

// Private methods

// Given a unknown ast node travsers the tree and return the resulting Value
Value* Interpreter::evaluate (Node* ast, EmuEnv* env) {

  switch (ast->type) {
  // Handle Literals and Symbols.
  case NodeType::NUMBER:
  case NodeType::BOOLEAN:
  case NodeType::SYMBOL:
  case NodeType::NULLISH:
    return evaluateLiteral(ast, env);
  case NodeType::LIST:
    // Handle lists / commands with arguments (set x 10 ) (+ 10 20) etc...
    return evaluateList((ListExpr*)ast, env);
  default:
    printf("Unexpected AST node encontered while travsersing the tree. val=( %d )\n", (int)ast->type);
    emu::throw_error("Cannot proceed further during evaluation\n");
    // this line is never called. However this is to ensure g++ dont get mad xD
    return evaluateLiteral(ast, env);
  }

}

// Evaluate a litral ast node to its literal value.
Value* Interpreter::evaluateLiteral (Node* node, EmuEnv* env) {
  Value* value;
  NodeType type = node->type;
  if (type == NodeType::NUMBER)
    value = CREATE_NUMBER_VAL(((NumericLiteral*)node)->value);
  else if (type == NodeType::NULLISH) 
    value = CREATE_NULL_VAL();

  else if (type == NodeType::SYMBOL) 
    value = evaluateSymbol((Symbol*)node, env);
  
  else if (type == NodeType::BOOLEAN)
    value = CREATE_BOOLEAN_VAL(((BooleanLiteral*)node)->value);

  // We will throw an error if the node type is undefined.
  else 
    emu::throw_error("Unexpected type found inside ast interpreter: LITERAL: ", 
                      std::to_string((int)node->type).c_str());
  
  return value;
}

// Performs a lookup for the variable or function. Will throw an error if the value is not valid.
Value* Interpreter::evaluateSymbol(Symbol* sym, EmuEnv* env) {

// Handle Variable access
  Value* v = env->find(sym->value);

  // Check for a valid variable lookup. If none is found then will return a nullptr.
  if (v == nullptr) {
    sym->print();
    emu::runtime_exception("Undetermined variable or operation found: ", sym->value.c_str());
  }
  return v;
}

// Evaluate a list expression. Things like a statement list are evaluated here as well as 
// more complex instructions like (+ 10 20) or nexted instructions.
Value* Interpreter::evaluateList(ListExpr* list, EmuEnv* env) {
  Value* v = CREATE_NULL_VAL();
  
  // Determine if the first element is a symbol. 
  // If its not then evaluate each element and return the last evaluated one.
  auto first = list->list[0];

  if (first->type != NodeType::SYMBOL) {
    for (auto expr : list->list)
      v = evaluate (expr, env);

    // return the last evaluated value.
    return v;
  }

  // name of the command/keyword. var, +, -=, fn, etc...
  std::string command = ((Symbol*)first)->value;

  // TODO ADD THIS AND OTHERS TO LIST OF NATIVE STD LIBRARY FUNCTIONS
  if (command.compare("+") == 0) {
    auto n1 = evaluate (list->list[1], env);
    auto n2 = evaluate (list->list[2], env);

    if (!IS_NUMBER(n1) || !IS_NUMBER(n2)) {
      std::cout << "Cannot perform binary operations on non numeric or string datatypes: ";
      PRINT_EMU_VALUE(n1);
      PRINT_EMU_VALUE(n2); 
      emu::runtime_exception("Invalid binary op\n");
    }

    auto result = TO_NUMBER(n1) + TO_NUMBER(n2);
    return CREATE_NUMBER_VAL (result);
  }
  ////////////////////////////
  // Variable Operations ////
  ///////////////////////////

  // (var x 100) -> set x to 100
  if (command.compare("var") == 0)
    return var_cmd(list, env);
  
  // (set x (+ x 100)) -> set x to 100 plus the prev value
  if (command.compare("set") == 0)
    return set_cmd(list, env);

  
  return evaluateSymbol((Symbol*)first, env);
}
