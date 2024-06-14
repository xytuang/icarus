#ifndef ICARUS_CLASS_H
#define ICARUS_CLASS_H

#include <string>
#include <vector>
#include <unordered_map>
#include <any>
#include <memory>

#include "icarus_callable.h"
#include "interpreter.h"

template <typename R>
class IcarusFunction;

class IcarusClass : public IcarusCallable, public enable_shared_from_this<IcarusClass> {
    public:
        std::string name;
        unordered_map<std::string, std::shared_ptr<IcarusFunction<std::any>>> methods;

        IcarusClass(std::string name, unordered_map<std::string, std::shared_ptr<IcarusFunction<std::any>>> methods);

        std::string toString();

        int arity();

        std::any call(Interpreter* interpreter, std::vector<std::any> arguments);

        std::shared_ptr<IcarusFunction<std::any>> findMethod(std::string name);

        std::shared_ptr<IcarusClass> getSharedPtr();

};


#endif
