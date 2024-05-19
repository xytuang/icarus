#ifndef SCANNER_H
#define SCANNER_H
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
    public:
        Scanner(std::string source) {}

        bool isAtEnd() {}

        std::vector<Token> scanTokens() {}

};
#endif
