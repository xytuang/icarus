#ifndef ICARUS_CALLABLE_H
#define ICARUS_CALLABLE_H

#include <vector>
#include <any>

#include "interpreter.h"

class IcarusCallable {
    public:
        virtual int arity() = 0;
        virtual std::any call(Interpreter* interpreter, std::vector<std::any> arguments) = 0;
};

#endif
