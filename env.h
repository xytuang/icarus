#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include <unordered_map>
#include <any>

#include "token.h"
#include "runtime_error.h"

class Environment {
    private:
        unordered_map<std::string, std::any> values;
    public:
        Environment(){}
        void define(std::string name, std::any value) {
            values[name] = value;
        }

        std::any get(Token* name) {
            if (values.find(name->getLexeme()) != values.end()) {
                return values[name->getLexeme()];
            }
            throw new RuntimeError(name, "Undefined variable: " + name->getLexeme() + ".");
        }

        ~Environment() = default;
};

#endif
