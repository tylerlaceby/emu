#include "parser.h"

Token Parser::current () {
    return tokens[pos];
}
Token Parser::peak () {
    assert(pos + 1 < tokens.size());
    return tokens[pos + 1];
}

Token Parser::eat () {
    // TODO Check for the valid size before trying to access. 
    // Make into actual check.
    assert(pos < tokens.size());
    auto c = current();
    pos++;
    return c;
}


Token Parser::eat (TokenType t, const char* errorMessage) {
    // TODO Check for the valid size before trying to access. 
    // Make into actual check.
    assert(pos < tokens.size());
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
    Program* program = new Program();
    tokens = lexer.tokenize(sourceCode);
    pos = 0;

    while (current() != TokenType::ENDFILE)
        program->body.push_back(statement());

    return program;
}

Statement* Parser::statement () {
    switch (current().type) {
    case TokenType::Let:
    case TokenType::Const:
        return variable_declaration_statement();

    case TokenType::LBrace:
        return block_statement();
    
    default:
        return expression();
    }
}

Statement* Parser::block_statement () {
    BlockStatement* stmt = new BlockStatement ();
    eat (); // left block

    while (current() != TokenType::RBrace && current() != TokenType::ENDFILE)
        stmt->body.push_back(statement());

    // right block
    eat(TokenType::RBrace, "Expected closing right brace inside block statement.\n");
    return stmt;

}

Statement* Parser::variable_declaration_statement () {

    bool isConst = (eat() == TokenType::Const);
    auto ident = eat(TokenType::Identifier, "Expectex token to be of type Identifier following let\n").symbol;
    eat(TokenType::Equals, "Inside var declaration you must provide a default value.\n, Must provide equals sign");
    auto expr = expression();

    return new VariableDeclaration(ident, expr, isConst);
}

Expression* Parser::variable_assignment_expression () {

    auto identifier = eat(TokenType::Identifier, "Only identifiers can be on the lhs of a variable assignment.").symbol;
    eat(); // eat equals;
    Expression* rhs = expression();

    return new VariableAssignment(identifier, rhs);
}

Expression* Parser::expression () {
    if (peak() == TokenType::LParen)
        return call_expression ();
    if (peak() == TokenType::Equals)
        return variable_assignment_expression();

    return comparison_expression();
}


Expression* Parser::call_expression () {
    // calle ...argsList
    // print (10, a, 10 + 20)
    auto identifierName = eat(TokenType::Identifier, "Expected a identifier before function call. Instead recieved non identifier.").symbol;
    auto args = comma_seperated_paren_expression();

    return new CallExpression(identifierName, args);
}   

std::vector<Expression*> Parser::comma_seperated_paren_expression() {
    eat(); // Eat left paren.
    std::vector<Expression*> callArgs = std::vector<Expression*>();
    while (current() != TokenType::RParen && current() != TokenType::ENDFILE) {

        callArgs.push_back(expression());
        if (current() == TokenType::Comma)
            eat();
        else if (current() == TokenType::RParen)
            continue;
        else {
            current().print();
            // Make sure commas seperate an expression.
            emu::error("Comma seperated lists must have comma seperating the list.");
        }
    }

    eat(TokenType::RParen, "Expected a right paren token.");       
    return callArgs;
}


Expression* Parser::comparison_expression () {
    Expression* lhs = additive_expression();

    while (current() == TokenType::DoubleEquals) {
        BinaryOp operation;
        auto c = eat().type;
        
        // Determine which operation to use.
        if (c == TokenType::DoubleEquals)
            operation = BinaryOp::IsEquals;

        Expression* rhs = additive_expression();
        BinaryExpression* bop = new BinaryExpression (lhs, operation, rhs);
        lhs = bop;
    }

    return lhs;
}

Expression* Parser::additive_expression () {

    Expression* lhs = multiplicative_expression ();

    while (current() == TokenType::Plus || current() == TokenType::Minus) {

        BinaryOp op = (eat() == TokenType::Plus)? BinaryOp::Add : BinaryOp::Subtract;

        Expression* right = multiplicative_expression();
        BinaryExpression* bop = new BinaryExpression(lhs, op, right);
        lhs = bop;
    }

    return lhs;
}

Expression* Parser::multiplicative_expression () {

    Expression* lhs = primary_expression();

    while (current() == TokenType::Asterisk || current() == TokenType::Slash) {

        BinaryOp op = (eat() == TokenType::Asterisk)? BinaryOp::Multiply : BinaryOp::Divide;

        Expression* right = primary_expression();
        BinaryExpression* bop = new BinaryExpression(lhs, op, right);

        lhs = bop;
    }

    return lhs;

}

LiteralExpression* Parser::literal_expression () {

    switch (current().type) {
    case TokenType::Numeric:
        return new NumericLiteral(eat().number);
    case TokenType::Nullish:
        eat();
        return new Null();
    case TokenType::Boolean:
        return new BooleanLiteral(eat().boolean);
    case TokenType::Identifier:
        return new Identifier(eat().symbol);
    default:
        current().print();
        emu::error("Parser Error!\n", "Expected Token to be of type LITERAL.");
        return new Null();
        break;
    }

}

Expression* Parser::primary_expression () {

    switch (current().type) {
    case TokenType::Nullish:
    case TokenType::Numeric:
    case TokenType::Boolean:
    case TokenType::Identifier:
        return literal_expression();
    
    case TokenType::LParen:
        return parenthesized_expression();
    default:
        printf(KRED "\nParsing Error:" RST);
        current().print();

        emu::error("Cannot continue parsing unexpected token found inside source.\n");
        return new Null();
    }
    

}


Expression* Parser::parenthesized_expression () {
    emu::error("Unimplimented Parenthesised Expression");
    return new Null();
}