#ifndef ICARUS_INSTANCE_H
#define ICARUS_INSTANCE_H


#include <unordered_map>
#include <any>
#include <string>
#include <memory>

#include "token.h"

class IcarusClass;

class IcarusInstance : public std::enable_shared_from_this<IcarusInstance> {
    private:
        std::weak_ptr<IcarusClass> klass;
        std::unordered_map<std::string, std::any> fields;
    public:
        IcarusInstance(std::shared_ptr<IcarusClass> klass);

        std::string toString();

        std::any get(std::shared_ptr<Token> name);

        void set(std::shared_ptr<Token> name, std::any value);

        std::shared_ptr<IcarusInstance> getSharedPtr();

};


#endif
