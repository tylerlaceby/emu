#ifndef EMU_PARSER_
#define EMU_PARSER_

#include "../emu.h"
#include "lexer.h"
#include "ast.h"

class Parser {
private:
    std::vector<Token> tokens;
    size_t pos;
    Token current ();
    Token eat ();
    Token eat (TokenType t, const char* errMessage);

public:

    Parser () {
        tokens = std::vector<Token>();
        pos = 0;
    }

    ListExpr* parse (std::string sourceCode);
    ListExpr* parseList ();
    Node* parseExpression ();
};

#endif