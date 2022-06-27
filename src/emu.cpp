#include "emu.h"
#include "parser/parser.h"


int main () {
    std::string code = emu::io::read_file_to_string ("tests/syntax.emu");
    Parser parser;
    ListExpr* tree = parser.parse(code);
    tree->print();
    return 0;
}