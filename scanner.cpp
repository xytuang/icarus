#include "scanner.h"
#include "errors.h"

class Scanner {
    private:
        const std::string source;
        const std::vector<Token> tokens;
        int start;
        int current;
        int line;
        unordered_map<std::string, TokenType> keywords;

        bool isAtEnd() {
            return this->current >= this->source.size();
        }

        bool isDigit(char c) {
            return c >= '0' && c <= '9';
        }

        bool isAlpha(char c){
            return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||  c == '_';
        }

        bool isAlphaNumeric(char c) {
            return isAlpha(c) || isDigit(c);
        }


        char  advance() {
            return this->source[current++];
        }

        void addToken(TokenType type, std::string literal) {
            std::string text = source.substr(start, current - start);
            tokens.push_back(new Token(type, text, literal, line));
        }

        void addToken(TokenType type) {
            addToken(type, "");
        }

        char peek() {
            if (isAtEnd()) return '\0';
            return source[current];
        }

        char peekNext() {
            if (current + 1 >= source.size()) return '\0';
            return source[current + 1];
        }

        bool match(char expected) {
            if (isAtEnd()) return false;
            if (source[current] != expected) return false;
            current++;
            return true;
        }


        void parseString() {
            while(peek() != '"' && !isAtEnd()) {
                if (peek() == '\n') line++;
                advance();
            }
            if (isAtEnd()) {
                error(line, "Unterminated String");
                return;
            }
            advance();
            std::string value = source.substr(start + 1, current - start - 2);
            addToken(STRING, value);
        }

        void parseNumber() {
            while (isDigit(peek())) advance();
            if (peek() == "." && isDigit(peekNext())) {
                advance();
                while(isDigit(peek())) advance();
            }
            addToken(NUMBER, std::stod(source.substr(start, current - start)));
        }


        void identifier() {
            while (isAlphaNumeric(peek())) advance();
            std::string text = source.substr(start, current);
            TokenType type;
            if (keywords.contains(text)) {
                type = keywords[text];
            }
            else {
                type = IDENTIFIER;
            }
            addToken(type);
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
                case '!': addToken(match('=') ? BANG_EQUAL : BANG); break;
                case '=': addToken(match('=') ? EQUAL_EQUAL : EQUAL); break;
                case '<': addToken(match('=') ? LESS_EQUAL : LESS); break;
                case '>': addToken(match('=') ? GREATER_EQUAL : GREATER); break;
                case '/':
                    if (match('/')) {
                      // A comment goes until the end of the line.
                      while (peek() != '\n' && !isAtEnd()) advance();
                    } 
                    else {
                      addToken(SLASH);
                    }
                    break;
                case ' ':
                case '\r':
                case '\t':
                    break;
                case '\n':
                    line++;
                    break;
                case '"': parseString(); break;
                default:
                    if (isDigit(c)) { 
                        parseNumber();
                    }
                    else if (isAlpha(c)) {
                        identifier();
                    }
                    else {
                        error(line, "Unexpected character."); break;
                    }
                    break;
            }
        }

    public:
        Scanner(std::string source) {
            this->source = source;
            this->start = 0;
            this->current = 0;
            this->line = 1;
            keywords = new HashMap<>();
            keywords["and"] = AND;
            keywords["class"] = CLASS;
            keywords["else"] = ELSE;
            keywords["false"] = FALSE;
            keywords["for"] = FOR;
            keywords["fun"] = FUN;
            keywords["if"] = IF;
            keywords["nil"] = NIL;
            keywords["or"] = OR;
            keywords["print"] = PRINT;
            keywords["return"] = RETURN;
            keywords["super"] = SUPER;
            keywords["this"] = THIS;
            keywords["true"] = TRUE;
            keywords["var"] = VAR;
            keywords["while"] = WHILE;
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
