#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include "expr.h"
#include "tokentype.h"
#include <exception>

class Parser {
    public:
        class ParseError : public std::exception {};
        Parser(std::vector<Token*> tokens);
        Expr* parse();

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

        Expr* primary();
        Expr* unary();
        Expr* factor();
        Expr* term();
        Expr* comparison();
        Expr* equality();
        Expr* expression();


};

#endif
