#ifndef EMU_TOKENS
#define EMU_TOKENS

#include "../emu.h"
#include <string>
#include <iostream>

// Types of EMULANG TOKENS
typedef enum class TokenType {
    NULLISH,
    NUMBER,
    SYMBOL,
    BOOLEAN,
    LPAREN,
    RPAREN,
    ENDFILE

} TokenType;

struct Token {

    TokenType type;
    
    std::string symbol;
    double number;
    bool boolean;

    // Constructors
    Token () {
        type = TokenType::NULLISH;
    }
    
    Token (TokenType t) {
        type = t;
    }

    Token (bool booleanVal) {
        type = TokenType::BOOLEAN;
        boolean = booleanVal;
    }

    Token (std::string& str) {
        symbol = str;
        type = TokenType::SYMBOL;
    }

    Token (double numeric) {
        number = numeric;
        type = TokenType::NUMBER;
    }

    bool operator== (TokenType t) { return type == t; }
    bool operator!= (TokenType t) { return type != t; }

    void print () {
        std::string t, v;

        if (type == TokenType::LPAREN)
            t = "Lparen";
        else if (type == TokenType::RPAREN)
            t = "Rparen";
        else if (type == TokenType::ENDFILE)
            t = "EOF";
        else if (type == TokenType::NULLISH)
            t = "Null";
        else if (type == TokenType::NUMBER) {
            t = "Number";
            v = std::to_string(number);
        } else if (type == TokenType::SYMBOL) {
            t = "Symbol";
            v = symbol;
        }

        else if (type == TokenType::BOOLEAN) {
            t = "Boolean";
            v = (boolean)? "true" : "false";
        }

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