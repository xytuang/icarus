#include "token.h"
#include "tokentype.h"
#include <any>

Token::Token(TokenType type, std::string lexeme, std::any literal, int line) {
    this->type = type;
    this->lexeme = lexeme;
    this->literal = literal;
    this->line = line;
}

std::any Token::getLiteral(){
    return this->literal;
}

std::string Token::getLexeme() {
    return this->lexeme;
}

int Token::getLine(){
    return this->line;
}

TokenType Token::getType() {
    return this->type;
}
