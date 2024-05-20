#ifndef ERRORS_H
#define ERRORS_H
#include <string>
#include <iostream>
extern bool hadError;

void report(int line, std::string where, std::string message) {
    std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
    hadError = true;
}

void error(int line, std::string message){
    report(line, "", message);
}

#endif
