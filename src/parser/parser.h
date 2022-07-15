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

private:
    Statement*  statement ();
    
    Statement* variable_declaration_statement ();
    Statement* function_declaration_statement ();
    Statement* block_statement ();
    Expression* expression ();

    Expression* call_expression ();
    Expression* object_expression ();
    Expression* variable_assignment_expression();
    Expression* comparison_expression ();
    Expression* additive_expression ();
    Expression* multiplicative_expression ();
    Expression* primary_expression ();
    LiteralExpression* literal_expression();
    Expression* parenthesized_expression ();


    // Helpers
    std::vector<Expression*> comma_seperated_paren_expression();
};

#endif