#ifndef ICARUS_FUNCTION_H
#define ICARUS_FUNCTION_H

#include <vector>
#include <any>
#include <iostream>

#include "icarus_callable.h"
#include "stmt.h"
#include "interpreter.h"
#include "env.h"
#include "stack_unwinder.h"
#include "icarus_instance.h"

template <typename R>
class IcarusFunction : public IcarusCallable {
    private:
        Function<R>* declaration;
        Environment* closure;
        bool isInitializer;
    public:
        IcarusFunction(Function<R>* declaration, Environment* closure, bool isInitializer) {
            this->closure = closure;
            this->declaration = declaration;
            this->isInitializer = isInitializer;
        }

        IcarusFunction* bind(IcarusInstance* instance) {
            Environment* environment = new Environment(closure);
            environment->define("this", instance);
            return new IcarusFunction(declaration, environment, isInitializer);
        }

        int arity() {
            return this->declaration->params.size();
        }

        std::any call(Interpreter* interpreter, std::vector<std::any> arguments) {
            Environment* environment = new Environment(this->closure);
            for (int i = 0; i < this->declaration->params.size(); i++) {
                environment->define(this->declaration->params[i]->getLexeme(), arguments[i]);
            }
            try {
                interpreter->executeBlock(this->declaration->body, environment);
            } catch (StackUnwinder* returnValue) {
                return returnValue->value;
            }
            if (isInitializer) {
                return closure->getAt(0, "this");
            }
            if (isInitializer) return closure->getAt(0, "this");
            return nullptr;
        }

        std::string toString() {
            return "<fn " + this->declaration->name->getLexeme() + ">";
        }
};



#endif
