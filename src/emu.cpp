#include "emu.h"
#include "parser/parser.h"
#include "runtime./interpreter.h"

int main () {
    std::string code = emu::io::read_file_to_string ("tests/syntax.emu");
    Parser parser;
    Interpreter runtime;

    // Ferform Lexing and Parsing
    ListExpr* tree = parser.parse(code);
    tree->print();

    // Evaluate tree with tree-walker
    Value* result = runtime.evaluate(tree);
    printf("Finished evaluation: resulttype: %d\n", (int)result->type);
    return 0;
}