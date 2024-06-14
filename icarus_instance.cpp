#include "icarus_instance.h"

#include <string>
#include <any>
#include <memory>

#include "icarus_class.h"
#include "icarus_function.h"
#include "runtime_error.h"
#include "token.h"

IcarusInstance::IcarusInstance(std::shared_ptr<IcarusClass> klass) {
    this->klass = klass;
}

std::string IcarusInstance::toString() {
    return this->klass->name + " instance";
}

std::any IcarusInstance::get(std::shared_ptr<Token> name) {
    if (this->fields.find(name->getLexeme()) != this->fields.end()) {
        return this->fields[name->getLexeme()];
    }
    std::shared_ptr<IcarusFunction<std::any>> method = this->klass->findMethod(name->getLexeme());
    if (method != nullptr) {
        return method->bind(getSharedPtr());
    }
    throw new RuntimeError(name, "Undefined property: " + name->getLexeme() + ".");
}

void IcarusInstance::set(std::shared_ptr<Token> name, std::any value) {
    this->fields[name->getLexeme()] = value;
}

std::shared_ptr<IcarusInstance> IcarusInstance::getSharedPtr() {
    return this->shared_from_this();
}
