#include <string>
#include <any>
#include <vector>
#include <memory>

#include "interpreter.h"
#include "icarus_class.h"
#include "icarus_function.h"
#include "icarus_instance.h"

IcarusClass::IcarusClass(std::string name, unordered_map<std::string, std::shared_ptr<IcarusFunction<std::any>>> methods) {
    this->name = name;
    this->methods = methods;
}

std::string IcarusClass::toString() {
    return this->name;
}


//arity of constructor
int IcarusClass::arity() {
    std::shared_ptr<IcarusFunction<std::any>> initializer = findMethod("init");
    if (initializer == nullptr) return 0;
    return initializer->arity();
}

std::any IcarusClass::call(Interpreter* interpreter, std::vector<std::any> arguments) {
    std::shared_ptr<IcarusInstance> instance = std::make_shared<IcarusInstance>(this);
    std::shared_ptr<IcarusFunction<std::any>> initializer = findMethod("init");
    if (initializer != nullptr) {
        initializer->bind(instance)->call(interpreter, arguments);
    }
    return instance;
}

std::shared_ptr<IcarusFunction<std::any>> IcarusClass::findMethod(std::string name) {
    if (this->methods.find(name) != this->methods.end()) {
        return this->methods[name];
    }
    return nullptr;
}

std::shared_ptr<IcarusClass> IcarusClass::getSharedPtr() {
    return this->shared_from_this();
}
