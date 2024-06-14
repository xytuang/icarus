#ifndef RESOLVER_H
#define RESOLVER_H

#include <vector>
#include <unordered_map>
#include <memory>

#include "token.h"
#include "expr.h"
#include "stmt.h"
#include "interpreter.h"
#include "enums.h"

class Resolver : public Expr<std::any>::Visitor<std::any>, public Stmt<std::any>::Visitor<std::any> {
    private:
        Interpreter* interpreter;

        std::vector<std::unordered_map<std::string, bool>> scopes;

        FunctionType currentFunction = FunctionType::NONE;


        void resolve(std::shared_ptr<Expr<std::any>> expr);

        void resolve(std::shared_ptr<Stmt<std::any>> stmt);

        void declare(std::shared_ptr<Token> name);

        void define(std::shared_ptr<Token> name);

        void resolveLocal(std::shared_ptr<Expr<std::any>> expr, std::shared_ptr<Token> name);

        void resolveFunction(std::shared_ptr<Function<std::any>> function, FunctionType type);

        void beginScope();

        void endScope();


    public:

        ClassType currentClass = ClassType::NONE;

        Resolver(Interpreter* interpreter);

        void resolve(std::vector<std::shared_ptr<Stmt<std::any>>> statements);

        std::any visitBlockStmt(std::shared_ptr<Block<std::any>> stmt);

        std::any visitClassStmt(std::shared_ptr<Class<std::any>> stmt);

        std::any visitVarStmt(std::shared_ptr<Var<std::any>> stmt);

        std::any visitFunctionStmt(std::shared_ptr<Function<std::any>> stmt);

        std::any visitExpressionStmt(std::shared_ptr<Expression<std::any>> stmt);

        std::any visitIfStmt(std::shared_ptr<If<std::any>> stmt);

        std::any visitPrintStmt(std::shared_ptr<Print<std::any>> stmt);

        std::any visitReturnStmt(std::shared_ptr<Return<std::any>> stmt);

        std::any visitWhileStmt(std::shared_ptr<While<std::any>> stmt);



        std::any visitAssignExpr(std::shared_ptr<Assign<std::any>> expr);

        std::any visitVariableExpr(std::shared_ptr<Variable<std::any>> expr);

        std::any visitBinaryExpr(std::shared_ptr<Binary<std::any>> expr);

        std::any visitCallExpr(std::shared_ptr<Call<std::any>> expr);

        std::any visitGetExpr(std::shared_ptr<Get<std::any>> expr);
 
        std::any visitGroupingExpr(std::shared_ptr<Grouping<std::any>> expr);

        std::any visitLiteralExpr(std::shared_ptr<Literal<std::any>> expr);

        std::any visitLogicalExpr(std::shared_ptr<Logical<std::any>> expr);

        std::any visitSetExpr(std::shared_ptr<Set<std::any>> expr);

        std::any visitThisExpr(std::shared_ptr<This<std::any>> expr);

        std::any visitUnaryExpr(std::shared_ptr<Unary<std::any>> expr);
};


#endif
