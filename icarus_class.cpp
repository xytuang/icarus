#include <string>
#include <any>
#include <vector>

#include "interpreter.h"
#include "icarus_class.h"
#include "icarus_function.h"
#include "icarus_instance.h"

IcarusClass::IcarusClass(std::string name, unordered_map<std::string, IcarusFunction<std::any>*> methods) {
    this->name = name;
    this->methods = methods;
}

std::string IcarusClass::toString() {
    return this->name;
}


//arity of constructor
int IcarusClass::arity() {
    IcarusFunction<std::any>* initializer = findMethod("init");
    if (initializer == nullptr) return 0;
    return initializer->arity();
}

std::any IcarusClass::call(Interpreter* interpreter, std::vector<std::any> arguments) {
    IcarusInstance* instance = new IcarusInstance(this);
    IcarusFunction<std::any>* initializer = findMethod("init");
    if (initializer != nullptr) {
        initializer->bind(instance)->call(interpreter, arguments);
    }
    return instance;
}

IcarusFunction<std::any>* IcarusClass::findMethod(std::string name) {
    if (this->methods.find(name) != this->methods.end()) {
        return this->methods[name];
    }
    return nullptr;
}
