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

    /**
     * @brief Given emu code will attempt to tokenize then build the AST tree.
     * This will not ensure proper grammer and runtime safety.
     */
    Program* parse (std::string& sourceCode);

private:

    /**
     * @brief Statements are the parent types for the emu language. All nodes inherit from the 
     * Statement ast type.
    */
    Statement*  statement ();
    
    /**
     * @brief Variable declarations must have a let or const keyword and follow  the pattern of
     * let/const `Equals` `Expression`
     * No default values are allowed as of now.
     * @code
     * let x = 45 # Valid emu declaration
     * const result = prompt("Enter a number: ") # Valid emu constant.
     * let y; # Invalid.
     * let nullishValue = null # Must declare null directly.
     * @endcode
     * 
     * 
    */
    Statement* variable_declaration_statement ();

    /**
     * @brief Function declarations follow the fn keyword. Then a comma seperated list of identifiers
     * is expected and then a block expression.
     * 
     * TODO?: Add support for a single line function where no block is needed to produce the body.
     */
    Statement* function_declaration_statement ();
    Statement* block_statement ();
    Expression* expression ();

    Expression* call_expression ();
    Expression* member_expression ();
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