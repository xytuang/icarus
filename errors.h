#ifndef ERRORS_H
#define ERRORS_H
#include <string>
#include <iostream>
#include "token.h"
#include "tokentype.h"

extern bool hadError;

void report(int line, std::string where, std::string message) {
    std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
    hadError = true;
}

void error(Token* token, std::string message){
    if (token->getType() == END_OF_FILE){
        report(token->getLine(), " at end ",  message);
    }
    else {
        report(token->getLine(), " at \'" + std::to_string(token->getLine()) + "\'", message);
    }
}

void error(int line, std::string message){

    report(line, "", message);
}

#endif
