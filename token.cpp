#include <token.h>
#include "tokentype.h" 
#include <string>
#include <ostream>

class Token {
    private:
        const TokenType type;
        const std::string lexeme;
        const std::string literal;
        const int line;
    public:
        Token(TokenType type, std::string lexeme, std::string literal, int line) {
            this->type = type;
            this->lexeme = lexeme;
            this->literal = literal;
            this->line = line;
        }
        //operator<< overload for std::ostream allows you to print a Token object using std::cout
        friend std::ostream& operator<<(std::ostream& os, const Token& token) {
            os << static_cast<int>(token.type) << " " << token.lexeme << " " << token.literal;
            return os;
        }
}

