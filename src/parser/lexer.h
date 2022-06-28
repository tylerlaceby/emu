#ifndef EMU_LEXER
#define EMU_LEXER

#include <vector>
#include <string>
#include "token.h"

class Lexer {

private:

    std::vector<Token> tokens;
    size_t pos;
    std::string source;

    bool notEOF () { return pos < source.size(); }
    char current () { return source[pos]; }

    bool isNumeric () {
        const char c = current();
        constexpr char mn = '0';
        constexpr char mx = '9';
        return (c >= mn && c <= mx);
    } 
    
    // check for characters that cannot be inside a identifier that could come up with bad spacing.
    bool isReserved () {
        const char c = current ();
        return (c == ' ' || c == '\n' || c == '(' || c == ')');
    }

public:

    std::vector<Token> tokenize (std::string src) {
        pos = 0;
        tokens = std::vector<Token>();
        source = src;
        source += "   ";

        buildTokens();

        return tokens;
    }

private:

    void addToken (TokenType t) { tokens.push_back(Token(t)); }

    void buildTokens () {

        while (notEOF()) {

            switch (current()) {
            case ' ' :
            case '\n':
                break;
            case '(':
                addToken(TokenType::LPAREN);
                break;
            case ')':
                addToken(TokenType::RPAREN);
                break;
            // Handle single line comments.
            case '#':
                while (notEOF() && current() != '\n')
                    pos++; 

                pos--;
                break;
            default: {
                    if (isNumeric()) {
                        // build numeric integers
                        std::string num;

                        while (notEOF() && isNumeric())
                            num += source[pos++];

                        tokens.push_back(Token(std::stod(num)));
                        pos--;
                        break;
                    }
                    
                    // at this point its a identifier
                    std::string ident;

                    while (notEOF() && !isReserved())
                        ident += source[pos++];

                    if (ident.compare("true") == 0)
                        tokens.push_back(Token(true));
                    else if (ident.compare("false") == 0)
                        tokens.push_back(Token(false));
                    else if (ident.compare("null") == 0)
                        tokens.push_back(Token(TokenType::NULLISH));
                    else  
                        tokens.push_back(Token(ident));
                        
                    pos--;
                    break;
                }
            }

            pos++;
        }

        tokens.push_back(Token(TokenType::ENDFILE));
    }   

};

#endif