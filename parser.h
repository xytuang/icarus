#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include "expr.h"
#include "tokentype.h"
#include <exception>

class Parser {
    private:
        class ParseError : public std::exception {};
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

        Expr* primary();
        Expr* unary();
        Expr* factor();
        Expr* term();
        Expr* comparison();
        Expr* equality();
        Expr* expression();

    public:
        Parser(std::vector<Token*> tokens);
        Expr* parse();

};

#endif
