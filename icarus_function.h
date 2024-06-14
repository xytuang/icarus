#ifndef ICARUS_FUNCTION_H
#define ICARUS_FUNCTION_H

#include <vector>
#include <any>
#include <iostream>
#include <memory>

#include "icarus_callable.h"
#include "icarus_instance.h"

#include "stmt.h"
#include "interpreter.h"
#include "env.h"
#include "stack_unwinder.h"

class IcarusInstance;

template <typename R>
class IcarusFunction : public IcarusCallable, public std::enable_shared_from_this<IcarusFunction<R>> {
    private:
        std::shared_ptr<Function<R>> declaration;
        Environment* closure;
        bool isInitializer;
    public:
        IcarusFunction(std::shared_ptr<Function<R>> declaration, Environment* closure, bool isInitializer);

        std::shared_ptr<IcarusFunction<R>> bind(std::shared_ptr<IcarusInstance> instance);

        int arity();

        std::any call(Interpreter* interpreter, std::vector<std::any> arguments);

        std::string toString();

        std::shared_ptr<IcarusFunction<R>> getSharedPtr();
        
};


template <typename R>
IcarusFunction<R>::IcarusFunction(std::shared_ptr<Function<R>> declaration, Environment* closure, bool isInitializer) {
    this->closure = closure;
    this->declaration = declaration;
    this->isInitializer = isInitializer;
}

template <typename R>
std::shared_ptr<IcarusFunction<R>> IcarusFunction<R>::bind(std::shared_ptr<IcarusInstance> instance) {
    Environment* environment = new Environment(closure);
    environment->define("this", instance);
    return std::make_shared<IcarusFunction>(declaration, environment, isInitializer);
}

template <typename R>
int IcarusFunction<R>::arity() {
    return this->declaration->params.size();
}

template <typename R>
std::any IcarusFunction<R>::call(Interpreter* interpreter, std::vector<std::any> arguments) {
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

template <typename R>
std::string IcarusFunction<R>::toString() {
    return "<fn " + this->declaration->name->getLexeme() + ">";
}

template <typename R>
std::shared_ptr<IcarusFunction<R>> IcarusFunction<R>::getSharedPtr() {
    return this->shared_from_this();
}
#endif
