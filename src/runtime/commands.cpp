#include "environment.h"
#include "interpreter.h"

  
// (symbol symbol value) or (var varname)
Value* Interpreter::var_cmd (ListExpr* list, EmuEnv* env) {
    std::vector<Node*> args = list->list;

    // check for only the command and no varname
    if (args.size() == 1) 
        emu::runtime_exception("var_cmd: To declare a variable you must haave atleast a variable name following the var keyword. Recieved None\n");
    // check for too many arguments.
    else if (args.size() > 3)
        emu::runtime_exception("Variable declaration has too many parameters. Should only include 2 arguments", "Example: (var varname value)\n");
    // Expected type to be a symbol for varname field.
    if (args[1]->type != NodeType::SYMBOL)
        emu::runtime_exception("Following a var keyword expect a variabla name\n Instead recieved: %d\n", std::to_string((int)args[1]->type).c_str());

    std::string varname = ((Symbol*)args[1])->value;
    
    // check if there is a third argument provided. If not use a null value to initialize variable.
    Value* val;
    if (args.size() == 2)
        val = CREATE_NULL_VAL();
    else val = evaluate (args[2], env);

    // attempt to declare the variable.
    env->declareVariable (varname, val);
    return val;
}

// (set symbol value)
Value* Interpreter::set_cmd (ListExpr* list, EmuEnv* env) {
    std::vector<Node*> args = list->list;

    // check for a valid set instruction of lenth 3
    if (args.size() != 3) 
        emu::runtime_exception("set_cmd: To assign a variable a new value you must provide the variable name followed by the value.\nExample: (set varname value)\n");
    
    if (args[1]->type != NodeType::SYMBOL)
        emu::runtime_exception("Cannot perform set instruction without a valid variable name\nRecieved: %d\n", std::to_string((int)args[1]->type).c_str());

    std::string varname = ((Symbol*)args[1])->value;
    Value* val = evaluate (args[2], env);
    env->assignVariable(varname, val);
    return val;
}