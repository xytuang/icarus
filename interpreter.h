#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <any>
#include <string>
#include <vector>

#include "expr.h"
#include "stmt.h"
#include "env.h"


class Interpreter : public Expr<std::any>::Visitor<std::any>, public Stmt<std::any>::Visitor<std::any> {
    private:

        std::any evaluate(Expr<std::any>* expr);
        std::any execute(Stmt<std::any>* stmt);

        bool isEqual(std::any A, std::any B);
        bool isTruthy(std::any object);
        void checkNumberOperand(Token* operation, std::any operand); 
        void checkNumberOperands(Token* operation, std::any left, std::any right);
        std::string stringify(std::any object);

    public:
        Environment* env;

        Interpreter();

        std::any visitLiteralExpr(Literal<std::any>* expr);
        std::any visitGroupingExpr(Grouping<std::any>* expr);
        std::any visitUnaryExpr(Unary<std::any>* expr);
        std::any visitBinaryExpr(Binary<std::any>* expr);

        std::any visitVariableExpr(Variable<std::any>* expr);


        std::any visitExpressionStmt(Expression<std::any>* stmt);
        std::any visitPrintStmt(Print<std::any>* stmt);

        std::any visitVarStmt(Var<std::any>* stmt);


        std::any interpret(std::vector<Stmt<std::any> *> statements);
        ~Interpreter() = default;

};

#endif
