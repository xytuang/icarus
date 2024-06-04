#include "icarus_class.h"

#include <string>
#include <any>
#include <vector>

#include "interpreter.h"
#include "icarus_instance.h"

IcarusClass::IcarusClass(std::string name) {
    this->name = name;
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
