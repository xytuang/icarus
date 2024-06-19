#ifndef CLEANER_H
#define CLEANER_H

#include <vector>
#include <any>

#include "expr.h"
#include "stmt.h"

class Cleaner : public Expr<std::any>::Visitor<std::any>, Stmt<std::any>::Visitor<std::any> {
    private:
    public:
        Cleaner();

        void clean(Expr<std::any>* stmt);
        void clean(Stmt<std::any>* stmt);
        void clean(std::vector<Stmt<std::any>*> statements);

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

};

#endif
