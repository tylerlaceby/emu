#ifndef EMU_TREE_WALK_ENV
#define EMU_TREE_WALK_ENV

#include "../emu.h"
#include "values.h"
#include <map>
#include <vector>

class EmuEnv {

private:
  EmuEnv* parent; // this could always be a nullptr if this is a global env

public:
  // Holds the list of all variables, custom types, functions etc...
  std::map<std::string, Value*> variables;
  
  EmuEnv (EmuEnv* p) {
    parent = p;
    variables = std::map<std::string, Value*>();
  }

  EmuEnv () {
    parent = nullptr;
    variables = std::map<std::string, Value*>();
  }

  // check for a variable to exist and return the value or a nullptr. This will check the parents as well until one is found.
  Value* find (std::string varname);
  // Given a variable name will return a nullptr or the environment that contains the current 
  // variable storage.
  EmuEnv* findEnv(std::string varname);

  /*
  * Define a variable in the current environment. If a value is already present this will throw a runtime error.
  */
  void declareVariable (std::string varname, Value* value);
  Value* assignVariable (std::string varname, Value* value);
};


static EmuEnv* CreateGlobalEnvironment () {

  EmuEnv* global = new EmuEnv();

  // CONSTANTS
  global->declareVariable("$version", CREATE_NUMBER_VAL(1));
  return global;
}


#endif