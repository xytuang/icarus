#include "icarus_instance.h"
#include <string>

#include "icarus_class.h"

IcarusInstance::IcarusInstance(IcarusClass* klass) {
    this->klass = klass;
}

std::string IcarusInstance::toString() {
    return this->klass->name + " instance";
}
