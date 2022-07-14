#include "parser.h"

Token Parser::current () {
    return tokens[pos];
}

Token Parser::eat () {
    auto c = current();
    pos++;
    return c;
}


Token Parser::eat (TokenType t, const char* errorMessage) {
    auto c = current();
    // check for incomplete types.
    if (c != t) {
        c.print();
        emu::error(errorMessage);
    }
    pos++;
    return c;
}

Program* Parser::parse (std::string& sourceCode) {
    Lexer lexer;
    Program* program;
    tokens = lexer.tokenize(sourceCode);
    pos = 0;

    program = new Program();
    while (current() != TokenType::ENDFILE)
        eat().print();

    return program;
}
