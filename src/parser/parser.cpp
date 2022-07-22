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
    case TokenType::If:
        return if_statement ();
    case TokenType::LBrace:
        return block_statement();
    case TokenType::Fn:
        return function_declaration_statement ();

    default:
        return expression();
    }
}

Statement* Parser::function_declaration_statement () {
    eat();

    std::string ident = eat(TokenType::Identifier, "Must have a named function in declaration").symbol;
    std::vector<Expression*> params = comma_seperated_paren_expression();
    std::vector<std::string> verifiedParams;
    // Verify all args are strings
    for (auto p : params) {
        if (p->type != NodeType::Identifier)
            emu::error("Cannot pass non identifier types into a function declaration.");


        std::string s = ((Identifier*)p)->value;
        verifiedParams.push_back(s);
    }


    auto body = (BlockStatement*) block_statement();
    FunctionDeclaration* fn = new FunctionDeclaration(ident, body, verifiedParams);
    return fn;
}

Statement* Parser::if_statement () {
    // Eat if token.
    eat(); 
    eat(TokenType::LParen, "Expected left paren inside if statement.");
    Expression* condition = expression();
    eat(TokenType::RParen, "Expected closing right paren inside if statement.");

    
    Statement* truthyBlock = statement();
    Statement* elseBlock = NULL;
    if (current() == TokenType::Else) {
        eat(); // eat else.
        elseBlock = statement();
    }
    
    return new IfStatement(condition, truthyBlock, elseBlock);
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



/* 
* expression
* assignment-expr
* or-expression
* and-expression
* comparison
* multiplicatave
* additive
* unary
* call-expression
* member-expression
* object-expression
* primary
* literal
* grouped
*/
Expression* Parser::expression () {
    return variable_assignment_expression();
}


Expression* Parser::variable_assignment_expression () {
    Expression* assigne = or_expression();
    // Handle case where no assignment takes place.
    if (current() != TokenType::Equals)
        return assigne;

    eat(); // eat equals;
    Expression* rhs = or_expression();
    return new VariableAssignment(assigne, rhs);
}



Expression* Parser::or_expression () {
    Expression* lhs = and_expression();

    while (current() == TokenType::Or ) {
        eat();
        BinaryOp operation = BinaryOp::Or;
        Expression* rhs = and_expression();
        BinaryExpression* bop = new BinaryExpression (lhs, operation, rhs);
        lhs = bop;
    }

    return lhs;
}


Expression* Parser::and_expression () {
    Expression* lhs = comparison_expression();
    
    while (current() == TokenType::And ) {
        eat();
        BinaryOp operation = BinaryOp::And;
        Expression* rhs = comparison_expression();
        BinaryExpression* bop = new BinaryExpression (lhs, operation, rhs);
        lhs = bop;
    }

    return lhs;
}


Expression* Parser::comparison_expression () {
    Expression* lhs = additive_expression();
    
    while (current() == TokenType::DoubleEquals || current() == TokenType::NotEquals
        || current() == TokenType::LessThan     || current() == TokenType::LessThanEq 
        || current() == TokenType::GreaterThan  || current() == TokenType::GreaterThanEq) {
        
        BinaryOp operation;
        auto c = eat().type;
        
        // Determine which operation to use.
        if (c == TokenType::DoubleEquals)
            operation = BinaryOp::IsEquals;
        else if (c == TokenType::NotEquals)
            operation = BinaryOp::NotEquals;
        // LessThan < & LessThanEq <=
        else if (c == TokenType::LessThan)
            operation = BinaryOp::LessThan;
        else if (c == TokenType::LessThanEq)
            operation = BinaryOp::LessThanEq;
        // GreaterThan > & GreaterThanEq >=
        else if (c == TokenType::GreaterThan)
            operation = BinaryOp::GreaterThan;
        else if (c == TokenType::GreaterThanEq)
            operation = BinaryOp::GreaterThanEq;

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

    Expression* lhs = unary_expression();

    while (current() == TokenType::Asterisk || current() == TokenType::Slash) {

        BinaryOp op = (eat() == TokenType::Asterisk)? BinaryOp::Multiply : BinaryOp::Divide;

        Expression* right = unary_expression();
        BinaryExpression* bop = new BinaryExpression(lhs, op, right);

        lhs = bop;
    }

    return lhs;

}


Expression* Parser::unary_expression () {
    if (current() == TokenType::Not || current() == TokenType::Minus) {
        UnaryOp op;
        auto o = eat();

        if (o == TokenType::Not)
            op = UnaryOp::Not;
        else op = UnaryOp::Inverse;

        Expression* term = call_expression();
        return new UnaryExpression (op, term);
    } 
    
    return call_expression();
}

Expression* Parser::call_expression () {
    Expression* calle = member_expression();
    
    if (current() != TokenType::LParen)
        return calle;

    auto args = comma_seperated_paren_expression();
    return new CallExpression(calle, args);
}  


std::vector<Expression*> Parser::comma_seperated_paren_expression() {
    eat(TokenType::LParen, "Expected a Left Parenthesis token for comma seperated list."); // Eat left paren.
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


// Member expression is a object.key
Expression* Parser::member_expression () {
    Expression* object = object_expression();
    while (current() == TokenType::Dot) {
        eat();
        Expression* property = object_expression();
        object = new MemberExpression (object, property);
    }
    

    return object;
}
    

Expression* Parser::object_expression () {

    if (current() != TokenType::LBrace)
        return primary_expression();

    eat();
    std::vector<ObjectProperty* > properties; 
    while (current() != TokenType::ENDFILE && current() != TokenType::RBrace) {

        std::string identifier = eat(TokenType::Identifier, "Expected a identifier for the key of a object literal.").symbol;
        Expression* val;
        bool shorthand = false;

        // check for shorthand.
        if (current() == TokenType::Comma || current() == TokenType::RBrace) {
            shorthand = true;
            val = new Null();
        } else {
            eat(TokenType::Colon, "Expected a colon and second arguments for Object LIteral");
            val = expression(); // THIS COULD BE ALOT LOWER DOWN?
        }
        // handle trailing commas {x: 10,} as well as commas from shorthand statements.
        if (current() == TokenType::Comma) eat();

        ObjectProperty* obj = new ObjectProperty (identifier, shorthand, val);
        properties.push_back(obj);
    }

    eat(TokenType::RBrace, "Expected a ending right brace.");
    return new ObjectExpression(properties);
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


Expression* Parser::parenthesized_expression () {
    eat(TokenType::LParen, "Expected left parenthesis - Parenthesised expr.");
    Expression* inner = expression();
    eat(TokenType::RParen, "Expected closing parenthesis inside parenthesised expression.");
    return inner;
}