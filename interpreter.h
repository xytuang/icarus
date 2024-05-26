#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <any>
#include "expr.h"

class Interpreter : public Expr<std::any>::Visitor<std::any> {
    private:
        std::any evaluate(Expr<std::any>* expr);
        bool isEqual(std::any A, std::any B);
        bool isTruthy(std::any object);


    public:
        std::any visitLiteralExpr(Literal<std::any>* expr);
        std::any visitGroupingExpr(Grouping<std::any>* expr);
        std::any visitUnaryExpr(Unary<std::any>* expr);
        std::any visitBinaryExpr(Binary<std::any>* expr);

        

};

#endif
