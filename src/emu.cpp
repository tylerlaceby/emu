#include "emu.h"
#include "parser/parser.h"
#include "runtime/Interpreter.h"
#include <string.h>

//
int main (const int argc, const char** args) {
    bool debugAST = false;
    bool repl = false;
    bool write_json_ast = false;


    // Handle CLI Args
    for (int i = 0; i < argc; i++) {
        if (strcmp(args[i], "--debug-ast") == 0)
            debugAST = true;
        if (strcmp(args[i], "--repl") == 0)
            repl = true;
        // If I ever add support for ast->json output
        if (strcmp(args[i], "--ast-json") == 0)
            write_json_ast = true;
    }


    Parser parser;
    Program* program;
    Interpreter interpreter;

    if (repl) {
        std::string buffer;
        std::string code;
        printf("\nemu language : version 0.2\n");
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

        // Handle runtime evaluation
        Val* result = interpreter.execute(program);
        display_value (result);
    }

    return 0;
}