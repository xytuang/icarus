#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include <unordered_map>
#include <any>
#include <iostream>
#include <memory>

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

        std::any get(std::shared_ptr<Token> name) {
            if (values.find(name->getLexeme()) != values.end()) {
                return values[name->getLexeme()];
            }
            if (enclosing != nullptr) {
                return enclosing->get(name);
            }
            throw new RuntimeError(name, "Undefined variable: " + name->getLexeme() + ".");
        }

        // part of resolution
        Environment* ancestor(int distance) {
            Environment* env = this;
            for (int i = 0; i < distance; i++) {
                env = env->enclosing;
            }
            return env;
        }

        // part of resolution
        std::any getAt(int distance, std::string name) {
            return ancestor(distance)->values[name];
        }

        // part of resolution
        void assignAt(int distance, std::shared_ptr<Token> name, std::any value) {
            ancestor(distance)->values[name->getLexeme()] = value;
        }

        void assign(std::shared_ptr<Token> name, std::any value) {
            if (values.find(name->getLexeme()) != values.end()) {
                values[name->getLexeme()] = value;
                return;
            }

            // if variable isn't in this environment, check the outer one recursively
            if (enclosing != nullptr) {
                enclosing->assign(name, value);
                return;
            }
            throw new RuntimeError(name, "Undefined variable: " + name->getLexeme() + ".");

        }

};

#endif
