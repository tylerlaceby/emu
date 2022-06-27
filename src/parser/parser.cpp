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
        emu::throw_error(errorMessage);
    }
    pos++;
    return c;
}

ListExpr* Parser::parse (std::string sourceCode) {
    Lexer lexer;
    tokens = lexer.tokenize(sourceCode);
    pos = 0;

    // for (auto token : tokens)
    //     token.print();

    ListExpr* list = new ListExpr ();

    while (current() != TokenType::ENDFILE)
        list->list.push_back(parseExpression());

    return list;
}

Node* Parser::parseExpression () {

    switch (current().type) {
    case TokenType::LPAREN:
        return parseList();
    case TokenType::NUMBER:
        return new NumericLiteral (eat().number);
    case TokenType::SYMBOL:
        return new Symbol (eat().symbol);
    case TokenType::NULLISH:
        eat();
        return new Null ();
    case TokenType::BOOLEAN:
        return new BooleanLiteral (eat().boolean);
    default:
        current().print();
        emu::throw_error("Reacted unexpected token");
        return new Null ();
    }

}

ListExpr* Parser::parseList () {
    eat(TokenType::LPAREN, "expected a left parenthesis token.");
    ListExpr* list = new ListExpr();

    while (current() != TokenType::RPAREN)
        list->list.push_back(parseExpression());
    
    eat(TokenType::RPAREN, "Expected a right parenthesis token following a list\n Intstead recieved.");
    return list;
}