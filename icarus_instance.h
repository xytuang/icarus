#ifndef ICARUS_INSTANCE_H
#define ICARUS_INSTANCE_H


#include <unordered_map>
#include <any>
#include <string>

#include "token.h"

class IcarusClass;

class IcarusInstance {
    private:
        IcarusClass* klass;
        std::unordered_map<std::string, std::any> fields;
    public:
        IcarusInstance(IcarusClass* klass);

        std::string toString();

        std::any get(Token* name);

        void set(Token* name, std::any value);

};


#endif
