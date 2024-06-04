#ifndef ICARUS_CLASS_H
#define ICARUS_CLASS_H

#include <string>
#include <vector>

#include "icarus_callable.h"
#include "interpreter.h"

class IcarusClass : public IcarusCallable {
    public:
        std::string name;
        IcarusClass(std::string name);

        std::string toString();

        int arity();
        std::any call(Interpreter* interpreter, std::vector<std::any> arguments);
};


#endif
