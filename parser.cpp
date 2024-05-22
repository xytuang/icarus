#include <string>
#include "parser.h"
#include "token.h"
#include "expr.h"

Parser::Parser(std::vector<Token*> tokens){
    this->tokens = tokens;
    this->current = 0;
}


Token* Parser::peek() {
    return this->tokens[this->current];
}

bool Parser::isAtEnd() {
    return peek()->getType() == END_OF_FILE;
}


Token* Parser::previous(){
    return this->tokens[this->current - 1];
}

Token* Parser::advance() {
    if (!isAtEnd()) {
        this->current++;
    }
    return previous();
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) {
        return false;
    }
    return peek()->getType() == type;
}

ParseError* Parser::error(Token* token, std::string message) {
    Icarus::error(token, message);
    return new ParseError();
}


Token* Parser::consume(TokenType type, std::string message) {
    if(check(type)) return advance();
    throw error(peek(), message);
}
void Parser::synchronize() {
    advance();
    while(!isAtEnd()) {
        if(previous()->getType() == SEMICOLON) return;
        
        switch (peek()->getType()){
            case CLASS:
            case FUN:
            case VAR:
            case FOR:
            case IF:
            case WHILE:
            case PRINT:
            case RETURN:
                return;
            default:
                break;
        }
        advance();
    }
}

bool Parser::match(std::initializer_list<TokenType> types) {
    std::vector<TokenType> vec(types);
    for (int i = 0; i < vec.size(); i++) {
        if (check(vec[i])){
            advance();
            return true;
        }
    }
    return false;
}

Expr* Parser::primary() {
    if (match({FALSE})) return new Literal("false");
    if (match({TRUE})) return new Literal("true");
    if (match({NIL})) return new Literal("");

    if (match({NUMBER, STRING})) return new Literal(previous()->getLiteral());

    if (match({LEFT_PAREN})) {
        Expr* expr = expression();
        consume(RIGHT_PAREN, "Expect \')\' after expression.");
        return new Grouping(expr);
    }

    throw error(peek(), "Expect expression");
}

Expr* Parser::unary() {
    while(match({BANG, MINUS})){
        Token* operation = previous();
        Expr* right = unary();
        return new Unary(operation, right);
    }
    return primary();
}

Expr* Parser::factor() {
    Expr* expr = unary();
    while(match({SLASH, STAR})) {
        Token* operation = previous();
        Expr* right = unary();
        expr = new Binary(expr, operation, right);
    }
    return expr;
}

Expr* Parser::term() {
    Expr* expr = factor();

    while(match({MINUS, PLUS})) {
        Token* operation = previous();
        Expr* right = factor();
        expr = new Binary(expr, operation, right);
    }
    return expr;
}

Expr* Parser::comparison() {
    Expr* expr = term();

    while(match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
        Token* operation = previous();
        Expr* right = term();
        expr = new Binary(expr, operation, right);
    }
    return expr;
}

Expr* Parser::equality() {
    Expr* expr = comparison();
    while(match({BANG_EQUAL, EQUAL_EQUAL})){
        Token* operation = previous();
        Expr* right = comparison();
        expr = new Binary(expr, operation, right);
    }
    return expr;
}

Expr* Parser::expression() {
    return equality();
}

Expr* Parser::parse() {
    try {
        return expression();
    } catch (ParseError error) {
        return nullptr;
    }
}
