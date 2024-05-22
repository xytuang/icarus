#include "token.h"
#include "tokentype.h"

Token::Token(TokenType type, std::string lexeme, std::string literal, int line) {
    this->type = type;
    this->lexeme = lexeme;
    this->literal = literal;
    this->line = line;
}

std::string Token::getLiteral(){
    return this->literal;
}

std::string Token::getLexeme() {
    return this->lexeme;
}

int Token::getLine(){
    return this->line;
}
