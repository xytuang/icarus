#ifndef TOKEN_H
#define TOKEN_H

#include <ostream>
#include <string>
#include <any>
#include "tokentype.h"

class Token {
    private:
        TokenType type;
        std::string lexeme;
        std::any literal;
        int line;
    public:
        Token(TokenType type, std::string lexeme, std::any literal, int line);
        //operator<< overload for std::ostream allows you to print a Token object using std::cout
        friend std::ostream& operator<<(std::ostream& os, const Token& token) {
            os << static_cast<int>(token.type) << " " << token.lexeme << " ";
            if (token.literal.has_value()) {
                if (token.literal.type() == typeid(std::string)) {
                    os << std::any_cast<std::string>(token.literal);
                }

                else if (token.literal.type() == typeid(int)) {
                    os << std::any_cast<int>(token.literal);
                }

                else if (token.literal.type() == typeid(double)) {
                    os << std::any_cast<double>(token.literal);
                }
                else {
                    os << "Unknown type";
                }
            }
            else {
                os << "null";
            }
            return os;
        }

        TokenType getType();

        std::string getLexeme();

        std::any getLiteral();

        int getLine();

};

#endif
