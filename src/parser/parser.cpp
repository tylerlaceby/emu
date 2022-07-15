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
        emu::error("Unimplimented variable declaration.");
        return new Null(); // here to shut compiler up.
        break;
    
    default:
        return expression();
    }
}


Expression* Parser::expression () {
    if (peak() == TokenType::LParen)
        emu::error("Have not implimented function calls");
    


    return comparison_expression();
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