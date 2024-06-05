#include "icarus_class.h"

#include <string>
#include <any>
#include <vector>

#include "interpreter.h"
#include "icarus_instance.h"
#include "icarus_function.h"

IcarusClass::IcarusClass(std::string name, unordered_map<std::string, IcarusFunction<std::any>*> methods) {
    this->name = name;
    this->methods = methods;
}

std::string IcarusClass::toString() {
    return this->name;
}


int IcarusClass::arity() {
    return 0;
}
std::any IcarusClass::call(Interpreter* interpreter, std::vector<std::any> arguments) {
    IcarusInstance* instance = new IcarusInstance(this);
    return instance;
}

IcarusFunction<std::any>* IcarusClass::findMethod(std::string name) {
    if (this->methods.find(name) != this->methods.end()) {
        return this->methods[name];
    }
    return nullptr;
}
