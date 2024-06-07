#include "icarus_function.h"

template <typename R>
IcarusFunction<R>::IcarusFunction(Function<R>* declaration, Environment* closure, bool isInitializer) {
    this->closure = closure;
    this->declaration = declaration;
    this->isInitializer = isInitializer;
}

template <typename R>
IcarusFunction<R>* IcarusFunction<R>::bind(IcarusInstance* instance) {
    Environment* environment = new Environment(closure);
    environment->define("this", instance);
    return new IcarusFunction(declaration, environment, isInitializer);
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
