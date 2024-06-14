#ifndef RUNTIME_ERROR_H
#define RUNTIME_ERROR_H

#include <stdexcept>
#include <string>
#include <memory>

#include "token.h"

class RuntimeError : public std::runtime_error {
public:
    std::shared_ptr<Token> token;
    RuntimeError(std::shared_ptr<Token> token, const std::string& message) : std::runtime_error(message), token(token) {}

};

#endif
