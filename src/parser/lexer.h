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
        auto c = current ();
        return (c == ' ' || c == '\n' || c == '(' || c == ')' || c == '+' || c == '-' || c == '/' || c == '*' || c == ':' || c == '=' || c == '?' || c == '!' || c == '{' || c == '}' || c == '[' || c == ']' || c == '.' || c == ',');
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

    const char peak () {
        if (pos + 1 >= source.size())
            emu::error("Lexical error!", "Attempted to read into invalid memory! Unexpected END OF FILE!", &source[source.size() - 1]);

        return source[pos + 1];
    }

    void buildTokens () {

        while (notEOF()) {

            switch (current()) {
            case ' ' :
            case '\n':
                break;
            case '(':
                addToken(TokenType::LParen);
                break;
            case ')':
                addToken(TokenType::RParen);
                break;
            case '.':
                addToken(TokenType::Dot);
                break;
            case '[':
                addToken(TokenType::LBracket);
                break;
            case ']':
                addToken(TokenType::RBracket);
                break;
            case ',':
                addToken(TokenType::Comma);
                break;
            case '{':
                addToken(TokenType::LBrace);
                break;
            case '}':
                addToken(TokenType::RBrace);
                break;
            case '!':
                addToken(TokenType::Not);
                break;
            case ':':
                addToken(TokenType::Colon);
                break;

            //////////////////////////////////////////////////////////////
            // Handle all operators that can have a single or double value
            //////////////////////////////////////////////////////////////

            case '=':
                
                if (peak() == '=') {
                    addToken(TokenType::DoubleEquals);
                    pos++;
                }
                else addToken(TokenType::Equals);
                break;

            case '+':

                if (peak() == '+') {
                    addToken(TokenType::DoublePlus);
                    pos++;
                }
                else addToken(TokenType::Plus);
                break;

            case '-':

                if (peak() == '-') {
                    addToken(TokenType::DoubleMinus);
                    pos++;
                }
                else addToken(TokenType::Minus);
                break;

            case '*':

                if (peak() == '*') {
                    addToken(TokenType::DoubleAsterisk);
                    pos++;
                }
                else addToken(TokenType::Asterisk);
                break;

            // Handle single slash. There is zero chance I will add a double slash ever lol!
            case '/':
                addToken(TokenType::Slash);
                break;
            
            
            // # is the beginning of a single line comment. All code and characters up to this point are 
            // not included inside tokenization.
            case '#':
                while (notEOF() && current() != '\n')
                    pos++; 

                // After locating the next line of code. We go back a position.
                // Ensures that upon the next iteration of the loop we will be at the start of the next line.
                pos--;
                break;
            default: {
                    
                    // Give priotity to the numeric values. Currently this will not include decimal places as those 
                    // Are not checked for in the inNumeric() method.
                    if (isNumeric()) {
                        // build numeric integers
                        std::string num;

                        while (notEOF() && isNumeric())
                            num += source[pos++];

                        tokens.push_back(Token(std::stold(num)));
                        pos--;
                        break;
                    }
                    
                    // at this point the character must be a identifier or a reserved word.
                    std::string ident;

                    while (notEOF() && !isReserved())
                        ident += source[pos++];


                    if (isReserved() && ident.size() == 0) {
                        printf("make sure token is added in switch statement as well as Token values: %c\nIdentifier: %s\n", current(), ident.c_str());
                        emu::error("Unsupported character is inside reserved list but is not handeled.\n");
                    }
                    ///////////////////////////
                    // Check for Literal Values
                    ///////////////////////////

                    if (ident.compare("true") == 0)
                        tokens.push_back(Token(true));
                    else if (ident.compare("false") == 0)
                        tokens.push_back(Token(false));
                    else if (ident.compare("null") == 0)
                        addToken(TokenType::Nullish);
                    
                    /////////////////////
                    // Check for Keywords
                    /////////////////////

                    else if (ident.compare("let") == 0)
                        tokens.push_back(Token(TokenType::Let));
                    else if (ident.compare("const") == 0)
                        tokens.push_back(Token(TokenType::Const));

                    
                    //////////////////////////
                    // Check for Other Symbols
                    //////////////////////////
                    
                    
                    ///////////////////////////////
                    // Lastly Assume Identifier.
                    ///////////////////////////////
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