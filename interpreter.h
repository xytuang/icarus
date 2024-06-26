#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <any>
#include <string>
#include <vector>
#include <unordered_map>

#include "expr.h"
#include "stmt.h"
#include "env.h"


class Interpreter : public Expr<std::any>::Visitor<std::any>, public Stmt<std::any>::Visitor<std::any> {
    private:

        std::any evaluate(Expr<std::any>* expr);
        std::any execute(Stmt<std::any>* stmt);

        std::any lookUpVariable(Token* name, Expr<std::any>* expr);

        bool isEqual(std::any A, std::any B);
        bool isTruthy(std::any object);
        void checkNumberOperand(Token* operation, std::any operand); 
        void checkNumberOperands(Token* operation, std::any left, std::any right);
        std::string stringify(std::any object);

    public:
        unordered_map<Expr<std::any>*, int> locals;
        Environment* env;
        Environment* globals;

        Interpreter();
        
        void resolve(Expr<std::any>* stmt, int depth);

        std::any executeBlock(std::vector<Stmt<std::any>*> statements, Environment* environment);

        std::any visitAssignExpr(Assign<std::any>* expr);
        std::any visitCallExpr(Call<std::any>* expr);
        std::any visitBinaryExpr(Binary<std::any>* expr);
        std::any visitGroupingExpr(Grouping<std::any>* expr);
        std::any visitLiteralExpr(Literal<std::any>* expr);
        std::any visitLogicalExpr(Logical<std::any>* expr);
        std::any visitUnaryExpr(Unary<std::any>* expr);
        std::any visitVariableExpr(Variable<std::any>* expr);

        std::any visitBlockStmt(Block<std::any>* stmt);
        std::any visitExpressionStmt(Expression<std::any>* stmt);
        std::any visitFunctionStmt(Function<std::any>* stmt); 
        std::any visitIfStmt(If<std::any>* stmt);
        std::any visitPrintStmt(Print<std::any>* stmt);
        std::any visitReturnStmt(Return<std::any>* stmt);
        std::any visitVarStmt(Var<std::any>* stmt);
        std::any visitWhileStmt(While<std::any>* stmt);

        std::any interpret(std::vector<Stmt<std::any> *> statements);
        ~Interpreter() = default;

};

#endif
