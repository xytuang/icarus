#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <any>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "expr.h"
#include "stmt.h"
#include "env.h"


class Interpreter : public Expr<std::any>::Visitor<std::any>, public Stmt<std::any>::Visitor<std::any> {
    private:

        std::any evaluate(std::shared_ptr<Expr<std::any>> expr);
        std::any execute(std::shared_ptr<Stmt<std::any>> stmt);
        std::any lookupVariable(std::shared_ptr<Token> name, std::shared_ptr<Expr<std::any>> expr);

        bool isEqual(std::any A, std::any B);
        bool isTruthy(std::any object);
        void checkNumberOperand(std::shared_ptr<Token> operation, std::any operand); 
        void checkNumberOperands(std::shared_ptr<Token> operation, std::any left, std::any right);
        std::string stringify(std::any object);

    public:
        Environment* env;
        Environment* globals;
        unordered_map<std::shared_ptr<Expr<std::any>>, int> locals;

        Interpreter();
        
        void resolve(std::shared_ptr<Expr<std::any>> expr, int depth);
        std::any executeBlock(std::vector<std::shared_ptr<Stmt<std::any>>> statements, Environment* environment);

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

        std::any interpret(std::vector<std::shared_ptr<Stmt<std::any>>> statements);
        ~Interpreter() = default;

};

#endif
