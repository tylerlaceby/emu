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


void EmuEnv::declareVariable (std::string varname, Value* value) {
  // check to make sure variable has no scope its declaraed in.
  if (findEnv(varname) != nullptr) 
    emu::runtime_exception("Variable with this name already is declared in the current scope. Cannot redeclare variable. Maybe assign it instead?\n", varname.c_str());
  
  // Define the variable if its not in any scope so far.
  variables[varname] = value;
}

Value* EmuEnv::assignVariable (std::string varname, Value* value) {
  EmuEnv* scope = findEnv(varname);
  // check for a valid scope first.
  if (scope == nullptr) 
    emu::runtime_exception("This variable name has not been declared in this scope or any parent scope. Please declare the variable before trying top assign it.\n", varname.c_str());
  scope->variables[varname] = value;

  return value;
}