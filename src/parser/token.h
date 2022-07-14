#ifndef EMU_TOKENS
#define EMU_TOKENS

#include "../emu.h"
#include <string>
#include <iostream>

// Types of EMULANG TOKENS
typedef enum class TokenType {
    // Literal Values & User Defined Literals
    Nullish,
    Numeric,
    Identifier,
    Boolean,
    // Reserved Words
    Let,

    // Reserved  Math Symbols
    Equals,
    DoubleEquals,

    Plus,
    Minus,
    Asterisk,
    Slash,
    DoublePlus,
    DoubleMinus,
    DoubleAsterisk,

    // Reserved Symbols
    LBracket,
    RBracket,
    Dot,
    Not,
    Colon,
    LParen,
    RParen,
    LBrace,
    RBrace,
    ENDFILE

} TokenType;

struct Token {

    TokenType type;
    
    std::string symbol;
    long double number;
    bool boolean;

    // Constructors
    Token () {
        type = TokenType::Nullish;
    }
    
    Token (TokenType t) {
        type = t;
    }

    Token (bool booleanVal) {
        type = TokenType::Boolean;
        boolean = booleanVal;
    }

    Token (std::string& str) {
        symbol = str;
        type = TokenType::Identifier;
    }

    Token (long double numeric) {
        number = numeric;
        type = TokenType::Numeric;
    }

    bool operator== (TokenType t) { return type == t; }
    bool operator!= (TokenType t) { return type != t; }

    void print () {
        std::string t, v;
        
        ////////////////////////
        // Literal Values Checks
        ////////////////////////

        if (type == TokenType::Nullish)
            t = "Null";
        else if (type == TokenType::Numeric) {
            t = "Number";
            v = std::to_string(number);
        } else if (type == TokenType::Identifier) {
            t = "Symbol";
            v = symbol;
        }

        else if (type == TokenType::Boolean) {
            t = "Boolean";
            v = (boolean)? "true" : "false";
        }

        ////////////////////
        // Reserved Keywords
        ////////////////////

        else if (type == TokenType::Let) 
            t = "Let";
        
        /////////////////////////
        // Reserved Math Keywords
        /////////////////////////
        
        else if (type == TokenType::Equals) 
            t = "Equals";
    
        else if (type == TokenType::DoubleEquals) 
            t = "DoubleEquals";
        
        else if (type == TokenType::Plus) 
            t = "Plus";
        
        else if (type == TokenType::DoublePlus) 
            t = "DoublePlus";

        else if (type == TokenType::Minus) 
            t = "Minus";
        else if (type == TokenType::DoubleMinus) 
            t = "DoubleMinus";
        else if (type == TokenType::Asterisk) 
            t = "Asterisk";
        else if (type == TokenType::DoubleAsterisk) 
            t = "DoubleAsterisk";
        else if (type == TokenType::Slash) 
            t = "Slash";

        //////////////////////////
        // Reserved Symbols Checks
        //////////////////////////

        else if (type == TokenType::Dot)
            t = "Dot";
        else if (type == TokenType::Not) 
            t = "Not";
        else if (type == TokenType::Colon) 
            t = "Colon";
        else if (type == TokenType::LBracket)
            t = "LBracket";
        else if (type == TokenType::RBracket)
            t = "RBracket";
        else if (type == TokenType::LParen)
            t = "Lparen";
        else if (type == TokenType::RParen)
            t = "Rparen";
            else if (type == TokenType::LBrace)
            t = "LBrace";
        else if (type == TokenType::RBrace)
            t = "RBrace";
        else if (type == TokenType::ENDFILE)
            t = "EOF";

        else {
            printf("token cannot print: %d\n", (int)type);
        }

        printf(" %s ( ", t.c_str());
        v.size() == 0 
            ? printf(")\n") 
            : printf(" %s )\n", v.c_str());
        
    }
};



#endif