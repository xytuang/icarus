#include "scanner.h"
#include <string>
#include <vector>
#include <unordered_map>
#include "tokentype.h"
#include "token.h"

class Scanner {
    private:
        const std::string source;
        const std::vector<Token> tokens;
        int start;
        int current;
        int line;

        char  advance() {
            return this->source[current++];
        }

        void addToken(TokenType type) {
            addToken(type, nullptr);
        }

        void scanToken(){
            char c = advance();
            switch (c) {
                case '(': addToken(LEFT_PAREN); break;
                case ')': addToken(RIGHT_PAREN); break;
                case '{': addToken(LEFT_BRACE); break;
                case '}': addToken(RIGHT_BRACE); break;
                case ',': addToken(COMMA); break;
                case '.': addToken(DOT); break;
                case '-': addToken(MINUS); break;
                case '+': addToken(PLUS); break;
                case ';': addToken(SEMICOLON); break;
                case '*': addToken(STAR); break; 
            }
        }


    public:
        Scanner(std::string source) {
            this->source = source;
            this->start = 0;
            this->current = 0;
            this->line = 1;
        }

        bool isAtEnd() {
            return this->current >= this->source.size();
        }

        std::vector<Token> scanTokens() {
            while(!isAtEnd()) {
                start = current;
                scanToken();
            }
            this->tokens.push_back(Token(END_OF_FILE, "", nullptr, line));
            return this->tokens;
        }

};
