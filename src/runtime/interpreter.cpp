#include "interpreter.h"
#include <vector>
#include <map>

Value* Interpreter::eval (Node* ast) {

  // define the global scope
  EmuEnv* env = CreateGlobalEnvironment();
  return evaluate(ast, env);
}

// Private methods

Value* Interpreter::evaluate (Node* ast, EmuEnv* env) {

  switch (ast->type) {
  case NodeType::NUMBER:
  case NodeType::BOOLEAN:
  case NodeType::SYMBOL:
  case NodeType::NULLISH:
    return evaluateLiteral(ast, env);
  case NodeType::LIST:
    return evaluateList((ListExpr*)ast, env);
  default:
    printf("Unexpected AST node encontered while travsersing the tree. val=( %d )\n", (int)ast->type);
    emu::throw_error("Cannot proceed further during evaluation\n");
    // this line is never called. However this is to ensure g++ dont get mad xD
    return evaluateLiteral(ast, env);
  }

}


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
  else 
    emu::throw_error("Unexpected type found inside ast interpreter: LITERAL: ", 
                      std::to_string((int)node->type).c_str());
  

  return value;
}

// Performs a lookup for the variable or function. Will throw an error if the value is not valid.
Value* Interpreter::evaluateSymbol(Symbol* sym, EmuEnv* env) {

// Handle Variable access
  Value* v = env->find(sym->value);
  if (v == nullptr) {
    sym->print();
    emu::runtime_exception("Undetermined variable or operation found: ", sym->value.c_str());
  }

  return v;
}

Value* Interpreter::evaluateList(ListExpr* list, EmuEnv* env) {
  Value* v = CREATE_NULL_VAL();

  auto first = list->list[0];

  // handle case where its just a bunch of blocks etc...
  if (first->type != NodeType::SYMBOL) {
    for (auto expr : list->list)
      v = evaluate (expr, env);

    // return the last evaluated value.
    return v;
  }

  Symbol command = *(Symbol*)first;
  
  // TODO ADD THIS AND OTHERS TO LIST OF NATIVE STD LIBRARY FUNCTIONS
  if (command.value.compare("+") == 0) {
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

  if (command.value.compare("var") == 0) {
    // add  checks for array bounds, undefined result coming back etc...
    if (list->list[1]->type != NodeType::SYMBOL)
      emu::runtime_exception("Expected identifier / variable name to come after variable keyword.\n");

    auto varname = ((Symbol*)(list->list[1]))->value;
    Value* resultValue = evaluate(list->list[2], env);
    env->declareVariable(varname, resultValue);
    return resultValue;
  }

  
  return evaluateSymbol(&command, env);
}