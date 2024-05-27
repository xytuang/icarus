#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <exception>

#include "expr.h"
#include "tokentype.h"
#include "stmt.h"

template <typename R>
class Parser {
    public:
        class ParseError : public std::exception {};
        Parser<R>(std::vector<Token*> tokens);
        std::vector<Stmt<R>*> parse();

    private:
        std::vector<Token *> tokens;
        int current;

        bool isAtEnd();
        Token* peek();
        Token* previous();
        Token* advance();
        bool check(TokenType type);

        ParseError* error(Token* token, std::string message);

        Token* consume(TokenType type, std::string message);

        void synchronize();

        bool match(std::initializer_list<TokenType> types);

        Expr<R>* primary();
        Expr<R>* unary();
        Expr<R>* factor();
        Expr<R>* term();
        Expr<R>* comparison();
        Expr<R>* equality();
        Expr<R>* expression();


        Stmt<R>* expressionStatement();
        Stmt<R>* printStatement();
        Stmt<R>* statement();


};

template <typename R>
Parser<R>::Parser(std::vector<Token*> tokens){
    this->tokens = tokens;
    this->current = 0;
}


template <typename R>
Token* Parser<R>::peek() {
    return this->tokens[this->current];
}

template <typename R>
bool Parser<R>::isAtEnd() {
    return peek()->getType() == END_OF_FILE;
}


template <typename R>
Token* Parser<R>::previous(){
    return this->tokens[this->current - 1];
}

template <typename R>
Token* Parser<R>::advance() {
    if (!isAtEnd()) {
        this->current++;
    }
    return previous();
}

template <typename R>
bool Parser<R>::check(TokenType type) {
    if (isAtEnd()) {
        return false;
    }
    return peek()->getType() == type;
}

template <typename R>
typename Parser<R>::ParseError* Parser<R>::error(Token* token, std::string message) {
    Icarus::error(token, message);
    return new Parser::ParseError();
}


template <typename R>
Token* Parser<R>::consume(TokenType type, std::string message) {
    if(check(type)) return advance();
    throw error(peek(), message);
}


template <typename R>
void Parser<R>::synchronize() {
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

template <typename R>
    bool Parser<R>::match(std::initializer_list<TokenType> types) {
    std::vector<TokenType> vec(types);
    for (int i = 0; i < vec.size(); i++) {
        if (check(vec[i])){
            advance();
            return true;
        }
    }
    return false;
}

template <typename R>
Expr<R>* Parser<R>::primary() {
    if (match({FALSE})) return new Literal<R>(false);
    if (match({TRUE})) return new Literal<R>(true);
    if (match({NIL})) return new Literal<R>(nullptr);

    if (match({NUMBER, STRING})) return new Literal<R>(previous()->getLiteral());

    if (match({LEFT_PAREN})) {
        Expr<R>* expr = expression();
        consume(RIGHT_PAREN, "Expect \')\' after expression.");
        return new Grouping(expr);
    }

    throw error(peek(), "Expect expression");
}

template <typename R>
Expr<R>* Parser<R>::unary() {
    while(match({BANG, MINUS})){
        Token* operation = previous();
        Expr<R>* right = unary();
        return new Unary<R>(operation, right);
    }
    return primary();
}

template <typename R>
Expr<R>* Parser<R>::factor() {
    Expr<R>* expr = unary();
    while(match({SLASH, STAR})) {
        Token* operation = previous();
        Expr<R>* right = unary();
        expr = new Binary<R>(expr, operation, right);
    }
    return expr;
}

template <typename R>
Expr<R>* Parser<R>::term() {
    Expr<R>* expr = factor();

    while(match({MINUS, PLUS})) {
        Token* operation = previous();
        Expr<R>* right = factor();
        expr = new Binary<R>(expr, operation, right);
    }
    return expr;
}

template <typename R>
Expr<R>* Parser<R>::comparison() {
    Expr<R>* expr = term();

    while(match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
        Token* operation = previous();
        Expr<R>* right = term();
        expr = new Binary<R>(expr, operation, right);
    }
    return expr;
}


template <typename R>
Expr<R>* Parser<R>::equality() {
    Expr<R>* expr = comparison();
    while(match({BANG_EQUAL, EQUAL_EQUAL})){
        Token* operation = previous();
        Expr<R>* right = comparison();
        expr = new Binary<R>(expr, operation, right);
    }
    return expr;
}

template <typename R>
Expr<R>* Parser<R>::expression() {
    return equality();
}

template <typename R>
Stmt<R>* Parser<R>::expressionStatement() {
    Expr<R>* expr = expression();
    consume(SEMICOLON, "Expect \';\' after a value");
    return new Print<R>(expr);
}

template <typename R>
Stmt<R>* Parser<R>::printStatement() {
    Expr<R>* value = expression();
    consume(SEMICOLON, "Expect \';\' after a value");
    return new Expression<R>(value);
}

template <typename R>
Stmt<R>* Parser<R>::statement() {
    if (match({PRINT})) {
        return printStatement();
    }
    return expressionStatement();
}

template <typename R>
std::vector<Stmt<R>*> Parser<R>::parse() {
    std::vector<Stmt<R>*> statements;
    while(!isAtEnd()){
        statements.push_back(statement());
    }
    return statements;
}
#endif
