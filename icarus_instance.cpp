#include "icarus_instance.h"

#include <string>
#include <any>

#include "icarus_class.h"
#include "runtime_error.h"
#include "token.h"

IcarusInstance::IcarusInstance(IcarusClass* klass) {
    this->klass = klass;
}

std::string IcarusInstance::toString() {
    return this->klass->name + " instance";
}

std::any IcarusInstance::get(Token* name) {
    if (this->fields.find(name->getLexeme()) != this->fields.end()) {
        return this->fields[name->getLexeme()];
    }
    IcarusFunction<std::any>* method = this->klass->findMethod(name->getLexeme());
    if (method != nullptr) {
        return method->bind(this);
    }
    throw new RuntimeError(name, "Undefined property: " + name->getLexeme() + ".");
}

void IcarusInstance::set(Token* name, std::any value) {
    this->fields[name->getLexeme()] = value;
}
