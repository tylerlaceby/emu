#include "interpreter.h"

Val* Interpreter::execute (Node* node) {
    // Create the global environment here.
    // TODOl: Possibly add suppport for passed env from main as it could
    // Allow statefull REPL.
    Env* env = new Env ();
    return eval (node, env);
}


Val* Interpreter::eval (Node* node, Env* env) {

    switch (node->type) {
    
    ///////////////////////////////////////////////
    //   Handle Main Program Vector<Statement*>  //
    //   Return last evaluated result.           //
    ///////////////////////////////////////////////

    case NodeType::Program: {
        Val* result;
        for (Statement* stmt : ((Program*)node)->body)
            result = eval(stmt, env);
        return result;
    }

    //////////////////////////////////
    // Parse Literals and Constants  /
    //////////////////////////////////

    case NodeType::NumericLiteral:
    case NodeType::BooleanLiteral:
    case NodeType::Null:
        return eval_literal ((LiteralExpression*)node, env);


    /////////////////////////////////////
    // Unimplimented Logic Travsersal  //
    /////////////////////////////////////

    default:
        printf("\nRuntimeError!\n");
        print_ast(node, 1);
        emu::runtime_exception("Unimplimented ast route: ");
        return new NumberVal(0);
    }

}

//////////////////////////////////
// Parse Literals and Constants  /
//////////////////////////////////

Val* Interpreter::eval_literal (LiteralExpression* node, Env* env) {

    /////////////
    // numbers  /
    /////////////

    if (node->type == NodeType::NumericLiteral) {
        long double number = ((NumericLiteral*)node)->value;
        return new NumberVal(number);
    } 

    //////////////
    // booleans  /
    //////////////

    else if (node->type == NodeType::BooleanLiteral) {
        bool boolvalue = ((BooleanLiteral*)node)->value;
        return new BoolVal (boolvalue);
    }

    ///////////////////
    // null literals  /
    ///////////////////

    else if (node->type == NodeType::Null) 
        return new NullVal ();
    

    // HANDLE UNIMPLIMENTED CONSTANT TYPES.
    // HANDLES FALLTHROUGH
    print_ast(node, 1);
    emu::runtime_exception("Unimplimented interpreter stage!");
    return new NumberVal(0); // so compiler wont complain.
}