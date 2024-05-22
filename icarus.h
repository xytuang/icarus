#ifndef ICARUS_H
#define ICARUS_H

#include <string>
#include "token.h"

class Icarus {
    public:
        static bool hadError;

        static void run(std::string source);

        static void runFile(char *filename);

        static void runPrompt();

        static void report(int line, std::string where, std::string message);

        static void error(Token* token, std::string message);
        
        static void error(int line, std::string message);
};

#endif
