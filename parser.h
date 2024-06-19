#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <exception>

#include <iostream>

#include "expr.h"
#include "tokentype.h"
#include "stmt.h"

template <typename R>
class Parser {
    public:

        std::vector<Stmt<R>*> statements;
        class ParseError : public std::exception {};
        Parser<R>(std::vector<Token*> tokens);
        std::vector<Stmt<R>*> parse();
        void clean();

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
        Expr<R>* finishCall(Expr<R>* callee);
        Expr<R>* call();
        Expr<R>* unary();
        Expr<R>* factor();
        Expr<R>* term();
        Expr<R>* comparison();
        Expr<R>* equality();
        Expr<R>* logicalAnd();
        Expr<R>* logicalOr();
        Expr<R>* assignment();
        Expr<R>* expression();


        Stmt<R>* expressionStatement();
        Stmt<R>* function(std::string kind);
        std::vector<Stmt<R>*> block();
        Stmt<R>* whileStatement();
        Stmt<R>* returnStatement();
        Stmt<R>* printStatement();
        Stmt<R>* ifStatement();
        Stmt<R>* forStatement();
        Stmt<R>* statement();

        Stmt<R>* varDeclaration();
        Stmt<R>* declaration();

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

    if(match({IDENTIFIER})) return new Variable<R>(previous());

    if (match({LEFT_PAREN})) {
        Expr<R>* expr = expression();
        consume(RIGHT_PAREN, "Expect \')\' after expression.");
        return new Grouping(expr);
    }

    throw error(peek(), "Expect expression");
}

template <typename R>
Expr<R>* Parser<R>::finishCall(Expr<R>* callee) {
    std::vector<Expr<R>*> arguments;
    if (!check(RIGHT_PAREN)) {
        do {
            if (arguments.size() >= 255) {
                error(peek(), "Can't have more than 255 arguments");
            }
            arguments.push_back(expression());
        }
        while (match({COMMA}));
    }
    Token* paren = consume(RIGHT_PAREN, "Expect \')\' after arguments");
    return new Call<R>(callee, paren, arguments);
}

template <typename R>
Expr<R>* Parser<R>::call() {
    Expr<R>* expr = primary();
    while (true) {
        if (match({LEFT_PAREN})) {
            expr = finishCall(expr);
        }
        else {
            break;
        }
    }
    return expr;
}

template <typename R>
Expr<R>* Parser<R>::unary() {
    while(match({BANG, MINUS})){
        Token* operation = previous();
        Expr<R>* right = unary();
        return new Unary<R>(operation, right);
    }
    return call();
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
Expr<R>* Parser<R>::logicalAnd() {
    Expr<R>* expr = equality();
    while (match({AND})) {
        Token* operation = previous();
        Expr<R>* right = equality();
        expr = new Logical(expr, operation, right);
    }
    return expr;
}

template <typename R>
Expr<R>* Parser<R>::logicalOr() {
    Expr<R>* expr = logicalAnd();
    while (match({OR})) {
        Token* operation = previous();
        Expr<R>* right = logicalAnd();
        expr = new Logical(expr, operation, right);
    }
    return expr;
}

template <typename R>
Expr<R>* Parser<R>::assignment() {
    Expr<R>* expr = logicalOr();
    if (match({EQUAL})) {
        Token* equals = previous();
        Expr<R>* value = assignment();

        if (dynamic_cast<Variable<std::any>*>(expr)) {
            Token* name = (dynamic_cast<Variable<std::any>*>(expr))->name;
            return new Assign<R>(name, value);

        }

        error(equals, "Invalid assignment target.");
    }
    return expr;

}
template <typename R>
Expr<R>* Parser<R>::expression() {
    return assignment();
}

template <typename R>
Stmt<R>* Parser<R>::function(std::string kind) {
    Token* name = consume(IDENTIFIER, "Expect " + kind + " name.");
    consume(LEFT_PAREN, "Expect \'(\' after " + kind + " name.");
    std::vector<Token*> parameters;
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
    std::vector<Stmt<R>*> body = block();
    return new Function<R>(name, parameters, body);
}




template <typename R>
Stmt<R>* Parser<R>::expressionStatement() {
    Expr<R>* expr = expression();
    consume(SEMICOLON, "Expect \';\' after expression");
    return new Expression<R>(expr);
}

template <typename R>
std::vector<Stmt<R>*> Parser<R>::block() {
    std::vector<Stmt<R>*> statements;
    while (!check(RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }
    consume(RIGHT_BRACE, "Expect \'}\' after block.");
    return statements;
}

template <typename R>
Stmt<R>* Parser<R>::whileStatement() {

    consume(LEFT_PAREN, "Expect \'(\' after \'while\'");
    Expr<R>* condition = expression();
    consume(RIGHT_PAREN, "Expect \')\' after while condition");

    Stmt<R>* body = statement();
    return new While<R>(condition, body);
}

template <typename R>
Stmt<R>* Parser<R>::returnStatement() {
    Token* keyword = previous();
    Expr<R>* value = nullptr;
    if (!check(SEMICOLON)) {
        value = expression();
    }
    consume(SEMICOLON, "Expect \';\' after return value");
    return new Return<R>(keyword, value);
}
template <typename R>
Stmt<R>* Parser<R>::printStatement() {
    Expr<R>* value = expression();
    consume(SEMICOLON, "Expect \';\' after a value");
    return new Print<R>(value);
}

template <typename R>
Stmt<R>* Parser<R>::ifStatement() {
    consume(LEFT_PAREN, "Expect \'(\' after \'if\'");
    Expr<R>* condition = expression();
    consume(RIGHT_PAREN, "Expect \')\' after if condition");

    Stmt<R>* thenBranch = statement();
    Stmt<R>* elseBranch = nullptr;
    if (match({ELSE})) {
        elseBranch = statement();
    }
    return new If<R>(condition, thenBranch, elseBranch);
}

template <typename R>
Stmt<R>* Parser<R>::forStatement() {
    consume(LEFT_PAREN, "Expect \'(\' after \'for\'");

    Stmt<R>* initializer;
    if (match({SEMICOLON})) {
        initializer = nullptr;
    }
    else if (match({VAR})) {
        initializer = varDeclaration();
    }
    else {
        initializer = expressionStatement();
    }

    Expr<R>* condition = nullptr;
    if (!check(SEMICOLON)) {
        condition = expression();
    }
    consume(SEMICOLON, "Expect \';\' after loop condition");


    Expr<R>* increment = nullptr;
    if (!check(RIGHT_PAREN)) {
        increment = expression();
    }
    consume(RIGHT_PAREN, "Expect \')\' after for clauses");

    Stmt<R>* body = statement();

    if (increment != nullptr) {
        std::vector<Stmt<R>*> statements;
        statements.push_back(body);
        statements.push_back(new Expression<R>(increment));
        body = new Block<R>(statements);
    }
    if (condition == nullptr) {
        condition = new Literal<R>(true);
    }
    body = new While<R>(condition, body);

    if (initializer != nullptr) {
        std::vector<Stmt<R>*> statements;
        statements.push_back(initializer);
        statements.push_back(body);
        body = new Block<R>(statements);
    }

    return body;
}

template <typename R>
Stmt<R>* Parser<R>::statement() {
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
        return new Block<R>(block());
    }
    return expressionStatement();
}

template <typename R>
Stmt<R>* Parser<R>::varDeclaration() {
    Token* name = consume(IDENTIFIER, "Expect variable name");

    Expr<R>* initializer = nullptr;
    if (match({EQUAL})) {
        initializer = expression();
    }

    consume(SEMICOLON, "Expect \';\' after variable declaration.");
    return new Var<R>(name, initializer);
}

template <typename R>
Stmt<R>* Parser<R>::declaration() {
    try {
        if (match({FUN})) return function("function");
        if (match({VAR})) return varDeclaration();
        return statement();
    } catch(ParseError* error) {
        synchronize();
        return nullptr;
    }
}


template <typename R>
std::vector<Stmt<R>*> Parser<R>::parse() {
    std::vector<Stmt<R>*> statements;
    while(!isAtEnd()){
        statements.push_back(declaration());
    }
    this->statements = statements;
    return statements;
}

template <typename R>
void Parser<R>::clean() {
    for (auto token : tokens) {
        delete token;
    }
}
#endif
