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

class IcarusInstance;

template <typename R>
class IcarusFunction : public IcarusCallable {
    private:
        Function<R>* declaration;
        Environment* closure;
        bool isInitializer;
    public:
        IcarusFunction(Function<R>* declaration, Environment* closure, bool isInitializer);

        IcarusFunction<R>* bind(IcarusInstance* instance);

        int arity();

        std::any call(Interpreter* interpreter, std::vector<std::any> arguments);

        std::string toString();
};



#endif
