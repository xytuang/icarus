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
        Environment* enclosing;
        Environment(){
            enclosing = nullptr;
        }

        Environment(Environment* enclosing){
            this->enclosing = enclosing;
        }

        void define(std::string name, std::any value) {
            values[name] = value;
        }

        Environment* ancestor(int distance) {
            Environment* env = this;
            for (int i = 0; i < distance; i++) {
                env = env->enclosing;
            }
            return env;
        }

        std::any getAt(int distance, std::string name) {
            return ancestor(distance)->values[name];
        }

        std::any get(Token* name) {
            if (values.find(name->getLexeme()) != values.end()) {
                return values[name->getLexeme()];
            }
            if (enclosing != nullptr) {
                return enclosing->get(name);
            }
            throw new RuntimeError(name, "Undefined variable: " + name->getLexeme() + ".");
        }

        void assignAt(int distance, Token* name, std::any value) {
            ancestor(distance)->values[name->getLexeme()] = value;
        }

        void assign(Token* name, std::any value) {
            if (values.find(name->getLexeme()) != values.end()) {
                values[name->getLexeme()] = value;
                return;
            }

            if (enclosing != nullptr) {
                enclosing->assign(name, value);
                return;
            }

            throw new RuntimeError(name, "Undefined variable: " + name->getLexeme() + ".");

        }

        ~Environment() = default;
};

#endif
