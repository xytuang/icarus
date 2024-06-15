#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <exception>

#include <iostream>

#include <memory>

#include "expr.h"
#include "tokentype.h"
#include "stmt.h"

template <typename R>
class Parser {
    public:
        class ParseError : public std::exception {};
        Parser<R>(std::vector<std::shared_ptr<Token>> tokens);
        std::vector<std::shared_ptr<Stmt<R>>> parse();

    private:
        std::vector<std::shared_ptr<Token>> tokens;
        int current;

        bool isAtEnd();
        std::shared_ptr<Token> peek();
        std::shared_ptr<Token> previous();
        std::shared_ptr<Token> advance();
        bool check(TokenType type);

        ParseError* error(std::shared_ptr<Token> token, std::string message);

        std::shared_ptr<Token> consume(TokenType type, std::string message);

        void synchronize();

        bool match(std::initializer_list<TokenType> types);

        std::shared_ptr<Expr<R>> primary();
        std::shared_ptr<Expr<R>> finishCall(std::shared_ptr<Expr<R>> callee);
        std::shared_ptr<Expr<R>> call();
        std::shared_ptr<Expr<R>> unary();
        std::shared_ptr<Expr<R>> factor();
        std::shared_ptr<Expr<R>> term();
        std::shared_ptr<Expr<R>> comparison();
        std::shared_ptr<Expr<R>> equality();
        std::shared_ptr<Expr<R>> logicalAnd();
        std::shared_ptr<Expr<R>> logicalOr();
        std::shared_ptr<Expr<R>> assignment();
        std::shared_ptr<Expr<R>> expression();


        std::shared_ptr<Stmt<R>> expressionStatement();
        std::shared_ptr<Stmt<R>> function(std::string kind);
        std::vector<std::shared_ptr<Stmt<R>>> block();
        std::shared_ptr<Stmt<R>> whileStatement();
        std::shared_ptr<Stmt<R>> returnStatement();
        std::shared_ptr<Stmt<R>> printStatement();
        std::shared_ptr<Stmt<R>> ifStatement();
        std::shared_ptr<Stmt<R>> forStatement();
        std::shared_ptr<Stmt<R>> statement();

        std::shared_ptr<Stmt<R>> classDeclaration();

        std::shared_ptr<Stmt<R>> varDeclaration();
        std::shared_ptr<Stmt<R>> declaration();

};

template <typename R>
Parser<R>::Parser(std::vector<std::shared_ptr<Token>> tokens){
    this->tokens = tokens;
    this->current = 0;
}


template <typename R>
std::shared_ptr<Token> Parser<R>::peek() {
    return this->tokens[this->current];
}

template <typename R>
bool Parser<R>::isAtEnd() {
    return peek()->getType() == END_OF_FILE;
}


template <typename R>
std::shared_ptr<Token> Parser<R>::previous(){
    return this->tokens[this->current - 1];
}

template <typename R>
std::shared_ptr<Token> Parser<R>::advance() {
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
typename Parser<R>::ParseError* Parser<R>::error(std::shared_ptr<Token> token, std::string message) {
    Icarus::error(token, message);
    return new Parser::ParseError();
}


template <typename R>
std::shared_ptr<Token> Parser<R>::consume(TokenType type, std::string message) {
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
std::shared_ptr<Expr<R>> Parser<R>::primary() {
    if (match({FALSE})) return std::make_shared<Literal<R>>(false);
    if (match({TRUE})) return std::make_shared<Literal<R>>(true);
    if (match({NIL})) return std::make_shared<Literal<R>>(nullptr);

    if (match({NUMBER, STRING})) return std::make_shared<Literal<R>>(previous()->getLiteral());
    if (match({THIS})) return std::make_shared<This<R>>(previous());
    if(match({IDENTIFIER})) return std::make_shared<Variable<R>>(previous());

    if (match({LEFT_PAREN})) {
        std::shared_ptr<Expr<R>> expr = expression();
        consume(RIGHT_PAREN, "Expect \')\' after expression.");
        return std::make_shared<Grouping<R>>(expr);
    }

    throw error(peek(), "Expect expression");
}

template <typename R>
std::shared_ptr<Expr<R>> Parser<R>::finishCall(std::shared_ptr<Expr<R>> callee) {
    std::vector<std::shared_ptr<Expr<R>>> arguments;
    if (!check(RIGHT_PAREN)) {
        do {
            if (arguments.size() >= 255) {
                error(peek(), "Can't have more than 255 arguments");
            }
            arguments.push_back(expression());
        }
        while (match({COMMA}));
    }
    std::shared_ptr<Token> paren = consume(RIGHT_PAREN, "Expect \')\' after arguments");
    return std::make_shared<Call<R>>(callee, paren, arguments);
}

template <typename R>
std::shared_ptr<Expr<R>> Parser<R>::call() {
    std::shared_ptr<Expr<R>> expr = primary();
    while (true) {
        if (match({LEFT_PAREN})) {
            expr = finishCall(expr);
        }
        else if (match({DOT})) {
            std::shared_ptr<Token> name = consume(IDENTIFIER, "Expect property name after \'.\'");
            expr = std::make_shared<Get<R>>(expr, name);
        }
        else {
            break;
        }
    }
    return expr;
}

template <typename R>
std::shared_ptr<Expr<R>> Parser<R>::unary() {
    while(match({BANG, MINUS})){
        std::shared_ptr<Token> operation = previous();
        std::shared_ptr<Expr<R>> right = unary();
        return std::make_shared<Unary<R>>(operation, right);
    }
    return call();
}

template <typename R>
std::shared_ptr<Expr<R>> Parser<R>::factor() {
    std::shared_ptr<Expr<R>> expr = unary();
    while(match({SLASH, STAR})) {
        std::shared_ptr<Token> operation = previous();
        std::shared_ptr<Expr<R>> right = unary();
        expr = std::make_shared<Binary<R>>(expr, operation, right);
    }
    return expr;
}

template <typename R>
std::shared_ptr<Expr<R>> Parser<R>::term() {
    std::shared_ptr<Expr<R>> expr = factor();
    while(match({MINUS, PLUS})) {
        std::shared_ptr<Token> operation = previous();
        std::shared_ptr<Expr<R>> right = factor();
        expr = std::make_shared<Binary<R>>(expr, operation, right);
    }
    return expr;
}

template <typename R>
std::shared_ptr<Expr<R>> Parser<R>::comparison() {
    std::shared_ptr<Expr<R>> expr = term();

    while(match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
        std::shared_ptr<Token> operation = previous();
        std::shared_ptr<Expr<R>> right = term();
        expr = std::make_shared<Binary<R>>(expr, operation, right);
    }
    return expr;
}


template <typename R>
std::shared_ptr<Expr<R>> Parser<R>::equality() {
    std::shared_ptr<Expr<R>> expr = comparison();
    while(match({BANG_EQUAL, EQUAL_EQUAL})){
        std::shared_ptr<Token> operation = previous();
        std::shared_ptr<Expr<R>> right = comparison();
        expr = std::make_shared<Binary<R>>(expr, operation, right);
    }
    return expr;
}

template <typename R>
std::shared_ptr<Expr<R>> Parser<R>::logicalAnd() {
    std::shared_ptr<Expr<R>> expr = equality();
    while (match({AND})) {
        std::shared_ptr<Token> operation = previous();
        std::shared_ptr<Expr<R>> right = equality();
        expr = std::make_shared<Logical<R>>(expr, operation, right);
    }
    return expr;
}

template <typename R>
std::shared_ptr<Expr<R>> Parser<R>::logicalOr() {
    std::shared_ptr<Expr<R>> expr = logicalAnd();
    while (match({OR})) {
        std::shared_ptr<Token> operation = previous();
        std::shared_ptr<Expr<R>> right = logicalAnd();
        expr = std::make_shared<Logical<R>>(expr, operation, right);
    }
    return expr;
}

template <typename R>
std::shared_ptr<Expr<R>> Parser<R>::assignment() {
    std::shared_ptr<Expr<R>> expr = logicalOr();
    if (match({EQUAL})) {
        std::shared_ptr<Token> equals = previous();
        std::shared_ptr<Expr<R>> value = assignment();
        if (dynamic_pointer_cast<Variable<R>>(expr)) {
            std::shared_ptr<Token> name = (dynamic_pointer_cast<Variable<R>>(expr))->name;
            return std::make_shared<Assign<R>>(name, value);
        }
        else if (dynamic_pointer_cast<Get<R>>(expr)) {
            std::shared_ptr<Get<R>> get = dynamic_pointer_cast<Get<R>>(expr);
            return std::make_shared<Set<R>>(get->object, get->name, value);
        }

        error(equals, "Invalid assignment target.");
    }
    return expr;

}
template <typename R>
std::shared_ptr<Expr<R>> Parser<R>::expression() {
    return assignment();
}

template <typename R>
std::shared_ptr<Stmt<R>> Parser<R>::function(std::string kind) {
    std::shared_ptr<Token> name = consume(IDENTIFIER, "Expect " + kind + " name.");
    consume(LEFT_PAREN, "Expect \'(\' after " + kind + " name.");
    std::vector<std::shared_ptr<Token>> parameters;
    if (!check(RIGHT_PAREN)) {
        do {
            if (parameters.size() >= 255) {
                error(peek(), "Can't have more than 255 parameters");
            }
            parameters.push_back(consume(IDENTIFIER, "Expect parameter name."));
        } while (match({COMMA}));
    }
    consume(RIGHT_PAREN, "Expect \')\' after parameters");

    consume(LEFT_BRACE, "Expect \'{\' before " + kind + " body.");
    std::vector<std::shared_ptr<Stmt<R>>> body = block();
    return std::make_shared<Function<R>>(name, parameters, body);
}



template <typename R>
std::shared_ptr<Stmt<R>> Parser<R>::expressionStatement() {
    std::shared_ptr<Expr<R>> expr = expression();
    consume(SEMICOLON, "Expect \';\' after expression");
    return std::make_shared<Expression<R>>(expr);
}

template <typename R>
std::vector<std::shared_ptr<Stmt<R>>> Parser<R>::block() {
    std::vector<std::shared_ptr<Stmt<R>>> statements;
    while (!check(RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }
    consume(RIGHT_BRACE, "Expect \'}\' after block.");
    return statements;
}

template <typename R>
std::shared_ptr<Stmt<R>> Parser<R>::whileStatement() {
    consume(LEFT_PAREN, "Expect \'(\' after \'while\'");
    std::shared_ptr<Expr<R>> condition = expression();
    consume(RIGHT_PAREN, "Expect \')\' after while condition");

    std::shared_ptr<Stmt<R>> body = statement();
    return std::make_shared<While<R>>(condition, body);
}

template <typename R>
std::shared_ptr<Stmt<R>> Parser<R>::returnStatement() {
    std::shared_ptr<Token> keyword = previous();
    std::shared_ptr<Expr<R>> value = nullptr;
    if (!check(SEMICOLON)) {
        value = expression();
    }
    consume(SEMICOLON, "Expect \';\' after return value");
    return std::make_shared<Return<R>>(keyword, value);
}
template <typename R>
std::shared_ptr<Stmt<R>> Parser<R>::printStatement() {
    std::shared_ptr<Expr<R>> value = expression();
    consume(SEMICOLON, "Expect \';\' after a value");
    return std::make_shared<Print<R>>(value);
}

template <typename R>
std::shared_ptr<Stmt<R>> Parser<R>::ifStatement() {
    consume(LEFT_PAREN, "Expect \'(\' after \'if\'");
    std::shared_ptr<Expr<R>> condition = expression();
    consume(RIGHT_PAREN, "Expect \')\' after if condition");

    std::shared_ptr<Stmt<R>> thenBranch = statement();
    std::shared_ptr<Stmt<R>> elseBranch = nullptr;
    if (match({ELSE})) {
        elseBranch = statement();
    }
    return std::make_shared<If<R>>(condition, thenBranch, elseBranch);
}

template <typename R>
std::shared_ptr<Stmt<R>> Parser<R>::forStatement() {
    consume(LEFT_PAREN, "Expect \'(\' after \'for\'");

    std::shared_ptr<Stmt<R>> initializer;
    if (match({SEMICOLON})) {
        initializer = nullptr;
    }
    else if (match({VAR})) {
        initializer = varDeclaration();
    }
    else {
        initializer = expressionStatement();
    }

    std::shared_ptr<Expr<R>> condition = nullptr;
    if (!check(SEMICOLON)) {
        condition = expression();
    }
    consume(SEMICOLON, "Expect \';\' after loop condition");


    std::shared_ptr<Expr<R>> increment = nullptr;
    if (!check(RIGHT_PAREN)) {
        increment = expression();
    }
    consume(RIGHT_PAREN, "Expect \')\' after for clauses");

    std::shared_ptr<Stmt<R>> body = statement();

    if (increment != nullptr) {
        std::vector<std::shared_ptr<Stmt<R>>> statements;
        statements.push_back(body);
        statements.push_back(std::make_shared<Expression<R>>(increment));
        body = std::make_shared<Block<R>>(statements);
    }
    if (condition == nullptr) {
        condition = std::make_shared<Literal<R>>(true);
    }
    body = std::make_shared<While<R>>(condition, body);

    if (initializer != nullptr) {
        std::vector<std::shared_ptr<Stmt<R>>> statements;
        statements.push_back(initializer);
        statements.push_back(body);
        body = std::make_shared<Block<R>>(statements);
    }

    return body;
}

template <typename R>
std::shared_ptr<Stmt<R>> Parser<R>::statement() {
    if (match({FOR})) {
        return forStatement();
    }
    if (match({IF})) {
        return ifStatement();
    }
    if (match({PRINT})) {
        return printStatement();
    }
    if (match({RETURN})) {
        return returnStatement();
    }
    if (match({WHILE})) {
        return whileStatement();
    }
    if (match({LEFT_BRACE})) {
        return std::make_shared<Block<R>>(block());
    }
    return expressionStatement();
}

template <typename R>
std::shared_ptr<Stmt<R>> Parser<R>::classDeclaration() {
    std::shared_ptr<Token> name = consume(IDENTIFIER, "Expect class name");
    consume(LEFT_BRACE, "Expect { before class declaration");
    std::vector<std::shared_ptr<Stmt<R>>> methods;
    while(!check(RIGHT_BRACE) && !isAtEnd()) {
        methods.push_back(function("method"));
    }

    consume(RIGHT_BRACE, "Expect } before class declaration");
    return std::make_shared<Class<R>>(name, methods);
}

template <typename R>
std::shared_ptr<Stmt<R>> Parser<R>::varDeclaration() {
    std::shared_ptr<Token> name = consume(IDENTIFIER, "Expect variable name");

    std::shared_ptr<Expr<R>> initializer = nullptr;
    if (match({EQUAL})) {
        initializer = expression();
    }

    consume(SEMICOLON, "Expect \';\' after variable declaration.");
    return std::make_shared<Var<R>>(name, initializer);
}

template <typename R>
std::shared_ptr<Stmt<R>> Parser<R>::declaration() {
    try {
        if (match({CLASS})) return classDeclaration();
        if (match({FUN})) return function("function");
        if (match({VAR})) return varDeclaration();
        return statement();
    } catch(ParseError* error) {
        synchronize();
        return nullptr;
    }
}


template <typename R>
std::vector<std::shared_ptr<Stmt<R>>> Parser<R>::parse() {
    std::vector<std::shared_ptr<Stmt<R>>> statements;
    while(!isAtEnd()){
        statements.push_back(declaration());
    }
    return statements;
}
#endif
