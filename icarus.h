#ifndef ICARUS_H
#define ICARUS_H

#include <string>
#include <memory>

#include "interpreter.h"
#include "token.h"
#include "runtime_error.h"

class Icarus {
    public:
        static Interpreter* interpreter;
        static bool hadError;

        static bool hadRuntimeError;

        static void run(std::string source);

        static void runFile(char *filename);

        static void runPrompt();

        static void report(int line, std::string where, std::string message);

        static void error(std::shared_ptr<Token> token, std::string message);
        
        static void error(int line, std::string message);

        static void runtimeError(RuntimeError* error);
};

#endif
