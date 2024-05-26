#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <any>
#include <string>
#include "expr.h"

class Interpreter : public Expr<std::any>::Visitor<std::any> {
    private:
        std::any evaluate(Expr<std::any>* expr);
        bool isEqual(std::any A, std::any B);
        bool isTruthy(std::any object);
        void checkNumberOperand(Token* operation, std::any operand); 
        void checkNumberOperands(Token* operation, std::any left, std::any right);
        std::string stringify(std::any object);


    public:
        std::any visitLiteralExpr(Literal<std::any>* expr);
        std::any visitGroupingExpr(Grouping<std::any>* expr);
        std::any visitUnaryExpr(Unary<std::any>* expr);
        std::any visitBinaryExpr(Binary<std::any>* expr);
        void interpret(Expr<std::any>* expr);
        ~Interpreter() = default;

        

};

#endif
