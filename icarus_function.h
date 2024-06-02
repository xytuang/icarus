#ifndef ICARUS_FUNCTION_H
#define ICARUS_FUNCTION_H

#include <vector>
#include <any>

#include "icarus_callable.h"
#include "stmt.h"
#include "interpreter.h"
#include "env.h"
#include "stack_unwinder.h"

template <typename R>
class IcarusFunction : public IcarusCallable {
    private:
        Function<R>* declaration;
    public:
        IcarusFunction(Function<R>* declaration) {
            this->declaration = declaration;
        }

        int arity() {
            return this->declaration->params.size();
        }

        std::any call(Interpreter* interpreter, std::vector<std::any> arguments) {
            Environment* environment = new Environment(interpreter->globals);
            for (int i = 0; i < this->declaration->params.size(); i++) {
                environment->define(this->declaration->params[i]->getLexeme(), arguments[i]);
            }
            try {
                interpreter->executeBlock(this->declaration->body, environment);
            } catch (StackUnwinder* returnValue) {
                return returnValue->value;
            }
            return nullptr;
        }

        std::string toString() {
            return "<fn " + this->declaration->name->lexeme + ">";
        }
};



#endif
