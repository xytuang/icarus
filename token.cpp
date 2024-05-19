#include "token.h"

Token::Token(TokenType type, std::string lexeme, std::string literal, int line) {
    this->type = type;
    this->lexeme = lexeme;
    this->literal = literal;
    this->line = line;
}
