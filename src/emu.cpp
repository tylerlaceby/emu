#include "emu.h"
#include "parser/parser.h"
#include "runtime/interpreter.h"

//
int main () {
    std::string code = emu::io::read_file_to_string ("tests/syntax.emu");
    Parser parser;

    Program* tree = parser.parse(code);
    print_ast(tree, 0);
    return 0;
}