#include "emu.h"
#include "parser/parser.h"
#include "runtime/interpreter.h"
#include <string.h>

//
int main (const int argc, const char** args) {
    bool debugAST = false;
    bool repl = false;

    // Handle CLI Args
    for (int i = 0; i < argc; i++) {
        if (strcmp(args[i], "--debug-ast") == 0)
            debugAST = true;
        if (strcmp(args[i], "--repl") == 0)
            repl = true;
    }


    Parser parser;
    Program* program;
    if (repl) {
        std::string buffer;
        std::string code;
        printf("\nemu language : version 0.1\n");
        while (true) {
            printf(">> ");
            std::getline(std::cin, buffer);
            if (buffer.compare("close()") == 0) break;
            else if (buffer.compare("exit()") == 0) break;
            else if (buffer.compare("quit()") == 0) break;

            code = std::string(buffer);
            program = parser.parse(code);
            if (debugAST) print_ast(program, 0);
        }
        
    } else {
        std::string code = emu::io::read_file_to_string ("tests/syntax.emu");
        program = parser.parse(code);
        if (debugAST) print_ast(program, 0);
    }
    return 0;
}