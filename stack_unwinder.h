#ifndef STACK_UNWINDER_H
#define STACK_UNWINDER_H

#include <exception>
#include <any>

class StackUnwinder : std::exception {
    public:
        std::any value;
        StackUnwinder(std::any value){
            this->value = value;
        }

        const char* what() const noexcept override {
            return "";
        }
        
};

#endif
