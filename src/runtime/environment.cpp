#include "environment.h"

Value* EmuEnv::find (std::string varname) {
  bool exists = variables.find(varname) != variables.end();

  if (exists)
    return variables[varname];

  // check for a parent. If none is present then the variable does not exist and return nullptr.
  if (parent == nullptr)
    return nullptr;

  return parent->find(varname);
}

EmuEnv* EmuEnv::findEnv (std::string varname) {
  bool exists = variables.find(varname) != variables.end();

  if (exists)
    return this;

  // check for a parent. If none is present then the variable does not exist and return nullptr.
  if (parent == nullptr)
    return nullptr;

  return parent->findEnv(varname);
}


void EmuEnv::declareVariable (std::string varname, Value* value, bool isConstant) {
  // Add variable if its not in the current scope.
  if (variables.find(varname) == variables.end()) {
    if (isConstant)
      constVars.insert(varname);
    variables[varname] = value;
  
    return;
  }
  
  emu::runtime_exception("Variable with this name already is declared in the current scope. Cannot redeclare variable. Maybe assign it instead?\n", varname.c_str());
}

Value* EmuEnv::assignVariable (std::string varname, Value* value) {
  EmuEnv* scope = findEnv(varname);
  // check for a valid scope first.
  if (scope == nullptr) 
    emu::runtime_exception("This variable name has not been declared in this scope or any parent scope. Please declare the variable before trying top assign it.\n", varname.c_str());
  
  // Make sure the variable is not a constant before assignment.
  bool isConstant = scope->constVars.find(varname) != scope->constVars.end();
  if (!isConstant) {
    scope->variables[varname] = value;
    return value;
  }
  

  // throw runtime error since variable is a constant.
  emu::runtime_exception("Variable is of type: constant and cannot be reassigned: ", varname.c_str());
  


  return value;
}