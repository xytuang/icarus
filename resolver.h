#ifndef RESOLVER_H
#define RESOLVER_H

#include <unordered_map>
#include <vector>
#include <any>
#include <string>

#include "expr.h"
#include "stmt.h"
#include "interpreter.h"
#include "token.h"


class Resolver : public Expr<std::any>::Visitor<std::any>, public Stmt<std::any>::Visitor<std::any> {
    private:
        Interpreter* interpreter;
        std::vector<unordered_map<std::string, bool>> scopes;

        void resolve(Stmt<std::any>* stmt);
        void resolve(Expr<std::any>* expr);
        void resolve(std::vector<Stmt<std::any>*> stmts);
        void resolveFunction(Function<std::any>* function);

        void beginScope(); 
        void endScope();

        void declare(Token* name);
        void define(Token* name);
        void resolveLocal(Expr<std::any>* expr, Token* name);

    public:
        Resolver(Interpreter* interpreter);

        std::any visitBlockStmt(Block<std::any>* stmt);

        std::any visitVarStmt(Var<std::any>* stmt);

        std::any visitVariableExpr(Variable<std::any>* stmt);

        std::any visitAssignExpr(Assign<std::any>* expr);

        std::any visitFunctionStmt(Function<std::any>* function);

        std::any visitExpressionStmt(Expression<std::any>* stmt);

        std::any visitIfStmt(If<std::any>* stmt);

        std::any visitPrintStmt(Print<std::any>* stmt);

        std::any visitReturnStmt(Return<std::any>* stmt);

        std::any visitWhileStmt(While<std::any>* stmt);

        std::any visitBinaryExpr(Binary<std::any>* stmt);

        std::any visitCallExpr(Call<std::any>* expr);

        std::any visitGroupingExpr(Grouping<std::any>* expr);

        std::any visitLiteralExpr(Literal<std::any>* expr);

        std::any visitLogicalExpr(Logical<std::any>* expr);

        std::any visitUnaryExpr(Unary<std::any>* expr);

};

#endif


