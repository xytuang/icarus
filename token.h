#ifndef TOKEN_H
#define TOKEN_H

#include <ostream>
#include <string>
#include "tokentype.h"

class Token {
    private:
        TokenType type;
        std::string lexeme;
        std::string literal;
        int line;
    public:
        Token(TokenType type, std::string lexeme, std::string literal, int line);
        //operator<< overload for std::ostream allows you to print a Token object using std::cout
        friend std::ostream& operator<<(std::ostream& os, const Token& token) {
            os << static_cast<int>(token.type) << " " << token.lexeme << " " << token.literal;
            return os;
        }

};

#endif
