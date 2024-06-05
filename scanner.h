#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <any>

#include "tokentype.h"
#include "token.h"

class Scanner {
    private:
        std::string source;
        std::vector<Token *> tokens;
        int start;
        int current;
        int line;
        std::unordered_map<std::string, TokenType> keywords;

        bool isAtEnd();

        bool isDigit(char c);

        bool isAlpha(char c);

        bool isAlphaNumeric(char c);

        char  advance();

        void addToken(TokenType type, std::any literal);

        void addToken(TokenType type);

        char peek();

        char peekNext();

        bool match(char expected);

        void parseString();

        void parseNumber();

        void identifier();

        void scanToken();

    public:
        Scanner(std::string source);

        std::vector<Token *> scanTokens();
};
#endif
