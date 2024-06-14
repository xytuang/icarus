#ifndef STMT_H
#define STMT_H
#include <vector>
#include <string>
#include <any>
#include <memory>
#include "token.h"
using namespace std;

template <typename R> class Block;
template <typename R> class Class;
template <typename R> class Expression;
template <typename R> class Function;
template <typename R> class If;
template <typename R> class Print;
template <typename R> class Return;
template <typename R> class Var;
template <typename R> class While;

template <typename R>
class Stmt{
public:
    template <typename T>
    class Visitor {
    public:
        virtual T visitBlockStmt (shared_ptr<Block<R>> stmt) = 0;
        virtual T visitClassStmt (shared_ptr<Class<R>> stmt) = 0;
        virtual T visitExpressionStmt (shared_ptr<Expression<R>> stmt) = 0;
        virtual T visitFunctionStmt (shared_ptr<Function<R>> stmt) = 0;
        virtual T visitIfStmt (shared_ptr<If<R>> stmt) = 0;
        virtual T visitPrintStmt (shared_ptr<Print<R>> stmt) = 0;
        virtual T visitReturnStmt (shared_ptr<Return<R>> stmt) = 0;
        virtual T visitVarStmt (shared_ptr<Var<R>> stmt) = 0;
        virtual T visitWhileStmt (shared_ptr<While<R>> stmt) = 0;
        virtual ~Visitor() = default;
    };

    virtual R accept(Visitor<R>* visitor) = 0;
    virtual ~Stmt() = default;
};

template <typename R>
class Block : public Stmt<R>, public enable_shared_from_this<Block<R>> {
public:
    vector<shared_ptr<Stmt<R>>> statements;
    Block(vector<shared_ptr<Stmt<R>>> statements) {
        this->statements=statements;
    }
    shared_ptr<Block<R>> getSharedPtr() {
        return this->shared_from_this();
    }
    R accept(typename Stmt<R>::template Visitor<R>* visitor) override {
        return visitor->visitBlockStmt(getSharedPtr());
    }
};

template <typename R>
class Class : public Stmt<R>, public enable_shared_from_this<Class<R>> {
public:
    shared_ptr<Token> name;
    vector<shared_ptr<Stmt<R>>> methods;
    Class(shared_ptr<Token> name, vector<shared_ptr<Stmt<R>>> methods) {
        this->name=name;
        this->methods=methods;
    }
    shared_ptr<Class<R>> getSharedPtr() {
        return this->shared_from_this();
    }
    R accept(typename Stmt<R>::template Visitor<R>* visitor) override {
        return visitor->visitClassStmt(getSharedPtr());
    }
};

template <typename R>
class Expression : public Stmt<R>, public enable_shared_from_this<Expression<R>> {
public:
    shared_ptr<Expr<R>> expression;
    Expression(shared_ptr<Expr<R>> expression) {
        this->expression=expression;
    }
    shared_ptr<Expression<R>> getSharedPtr() {
        return this->shared_from_this();
    }
    R accept(typename Stmt<R>::template Visitor<R>* visitor) override {
        return visitor->visitExpressionStmt(getSharedPtr());
    }
};

template <typename R>
class Function : public Stmt<R>, public enable_shared_from_this<Function<R>> {
public:
    shared_ptr<Token> name;
    vector<shared_ptr<Token>> params;
    vector<shared_ptr<Stmt<R>>> body;
    Function(shared_ptr<Token> name, vector<shared_ptr<Token>> params, vector<shared_ptr<Stmt<R>>> body) {
        this->name=name;
        this->params=params;
        this->body=body;
    }
    shared_ptr<Function<R>> getSharedPtr() {
        return this->shared_from_this();
    }
    R accept(typename Stmt<R>::template Visitor<R>* visitor) override {
        return visitor->visitFunctionStmt(getSharedPtr());
    }
};

template <typename R>
class If : public Stmt<R>, public enable_shared_from_this<If<R>> {
public:
    shared_ptr<Expr<R>> condition;
    shared_ptr<Stmt<R>> thenBranch;
    shared_ptr<Stmt<R>> elseBranch;
    If(shared_ptr<Expr<R>> condition, shared_ptr<Stmt<R>> thenBranch, shared_ptr<Stmt<R>> elseBranch) {
        this->condition=condition;
        this->thenBranch=thenBranch;
        this->elseBranch=elseBranch;
    }
    shared_ptr<If<R>> getSharedPtr() {
        return this->shared_from_this();
    }
    R accept(typename Stmt<R>::template Visitor<R>* visitor) override {
        return visitor->visitIfStmt(getSharedPtr());
    }
};

template <typename R>
class Print : public Stmt<R>, public enable_shared_from_this<Print<R>> {
public:
    shared_ptr<Expr<R>> expression;
    Print(shared_ptr<Expr<R>> expression) {
        this->expression=expression;
    }
    shared_ptr<Print<R>> getSharedPtr() {
        return this->shared_from_this();
    }
    R accept(typename Stmt<R>::template Visitor<R>* visitor) override {
        return visitor->visitPrintStmt(getSharedPtr());
    }
};

template <typename R>
class Return : public Stmt<R>, public enable_shared_from_this<Return<R>> {
public:
    shared_ptr<Token> keyword;
    shared_ptr<Expr<R>> value;
    Return(shared_ptr<Token> keyword, shared_ptr<Expr<R>> value) {
        this->keyword=keyword;
        this->value=value;
    }
    shared_ptr<Return<R>> getSharedPtr() {
        return this->shared_from_this();
    }
    R accept(typename Stmt<R>::template Visitor<R>* visitor) override {
        return visitor->visitReturnStmt(getSharedPtr());
    }
};

template <typename R>
class Var : public Stmt<R>, public enable_shared_from_this<Var<R>> {
public:
    shared_ptr<Token> name;
    shared_ptr<Expr<R>> initializer;
    Var(shared_ptr<Token> name, shared_ptr<Expr<R>> initializer) {
        this->name=name;
        this->initializer=initializer;
    }
    shared_ptr<Var<R>> getSharedPtr() {
        return this->shared_from_this();
    }
    R accept(typename Stmt<R>::template Visitor<R>* visitor) override {
        return visitor->visitVarStmt(getSharedPtr());
    }
};

template <typename R>
class While : public Stmt<R>, public enable_shared_from_this<While<R>> {
public:
    shared_ptr<Expr<R>> condition;
    shared_ptr<Stmt<R>> body;
    While(shared_ptr<Expr<R>> condition, shared_ptr<Stmt<R>> body) {
        this->condition=condition;
        this->body=body;
    }
    shared_ptr<While<R>> getSharedPtr() {
        return this->shared_from_this();
    }
    R accept(typename Stmt<R>::template Visitor<R>* visitor) override {
        return visitor->visitWhileStmt(getSharedPtr());
    }
};

#endif
