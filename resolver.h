#ifndef RESOLVER_H
#define RESOLVER_H

#include <vector>
#include <unordered_map>

#include "token.h"
#include "expr.h"
#include "stmt.h"
#include "interpreter.h"
#include "enums.h"

class Resolver : public Expr<std::any>::Visitor<std::any>, public Stmt<std::any>::Visitor<std::any> {
    private:
        Interpreter* interpreter;

        std::vector<std::unordered_map<std::string, bool>> scopes;

        FunctionType currentFunction = NONE;

        void resolve(Expr<std::any>* expr);

        void resolve(Stmt<std::any>* stmt);

        void declare(Token* name);

        void define(Token* name);

        void resolveLocal(Expr<std::any>* expr, Token* name);

        void resolveFunction(Function<std::any>* function, FunctionType type);

        void beginScope();

        void endScope();


    public:
        Resolver(Interpreter* interpreter);

        void resolve(std::vector<Stmt<std::any>*> statements);

        std::any visitBlockStmt(Block<std::any>* stmt);

        std::any visitVarStmt(Var<std::any>* stmt);

        std::any visitFunctionStmt(Function<std::any>* stmt);

        std::any visitExpressionStmt(Expression<std::any>* stmt);

        std::any visitIfStmt(If<std::any>* stmt);

        std::any visitPrintStmt(Print<std::any>* stmt);

        std::any visitReturnStmt(Return<std::any>* stmt);

        std::any visitWhileStmt(While<std::any>* stmt);



        std::any visitAssignExpr(Assign<std::any>* expr);

        std::any visitVariableExpr(Variable<std::any>* expr);

        std::any visitBinaryExpr(Binary<std::any>* expr);

        std::any visitCallExpr(Call<std::any>* expr);
 
        std::any visitGroupingExpr(Grouping<std::any>* expr);

        std::any visitLiteralExpr(Literal<std::any>* expr);

        std::any visitLogicalExpr(Logical<std::any>* expr);

        std::any visitUnaryExpr(Unary<std::any>* expr);
};


#endif
