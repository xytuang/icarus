#ifndef ICARUS_CLASS_H
#define ICARUS_CLASS_H

#include <string>
#include <vector>
#include <unordered_map>
#include <any>

#include "icarus_callable.h"
#include "interpreter.h"

class IcarusFunction;

class IcarusClass : public IcarusCallable {
    public:
        std::string name;
        unordered_map<std::string, IcarusFunction<std::any>* > methods;

        IcarusClass(std::string name, unordered_map<std::string, IcarusFunction<std::any>*> methods);

        std::string toString();

        int arity();

        std::any call(Interpreter* interpreter, std::vector<std::any> arguments);

        IcarusFunction<std::any>* findMethod(std::string name);

};


#endif
