#ifndef EMU_PARSER_
#define EMU_PARSER_

#include <assert.h>
#include "../emu.h"
#include "lexer.h"
#include "ast.h"

class Parser {
private:
    std::vector<Token> tokens;
    size_t pos;
    Token current ();
    Token peak ();
    Token eat ();
    Token eat (TokenType t, const char* errMessage);

public:

    Parser () {
        tokens = std::vector<Token>();
        pos = 0;
    }

    Program* parse (std::string& sourceCode);
    Statement*  statement ();
    Expression* expression ();
    Expression* comparison_expression ();
    Expression* additive_expression ();
    Expression* multiplicative_expression ();
    Expression* primary_expression ();
    LiteralExpression* literal_expression();
    Expression* parenthesized_expression ();
};

#endif