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
        virtual T visitBlockStmt (Block<R>* stmt) = 0;
        virtual T visitClassStmt (Class<R>* stmt) = 0;
        virtual T visitExpressionStmt (Expression<R>* stmt) = 0;
        virtual T visitFunctionStmt (Function<R>* stmt) = 0;
        virtual T visitIfStmt (If<R>* stmt) = 0;
        virtual T visitPrintStmt (Print<R>* stmt) = 0;
        virtual T visitReturnStmt (Return<R>* stmt) = 0;
        virtual T visitVarStmt (Var<R>* stmt) = 0;
        virtual T visitWhileStmt (While<R>* stmt) = 0;
        virtual ~Visitor() = default;
    };

    virtual R accept(Visitor<R>* visitor) = 0;
    virtual ~Stmt() = default;
};

template <typename R>
class Block : public Stmt<R> {
public:
    vector<shared_ptr<Stmt<R>>> statements;
    Block(vector<shared_ptr<Stmt<R>>> statements) {
        this->statements=statements;
    }
    R accept(typename Stmt<R>::template Visitor<R>* visitor) override {
        return visitor->visitBlockStmt(this);
    }
};

template <typename R>
class Class : public Stmt<R> {
public:
    shared_ptr<Token> name;
    vector<shared_ptr<Stmt<R>>> methods;
    Class(shared_ptr<Token> name, vector<shared_ptr<Stmt<R>>> methods) {
        this->name=name;
        this->methods=methods;
    }
    R accept(typename Stmt<R>::template Visitor<R>* visitor) override {
        return visitor->visitClassStmt(this);
    }
};

template <typename R>
class Expression : public Stmt<R> {
public:
    shared_ptr<Expr<R>> expression;
    Expression(shared_ptr<Expr<R>> expression) {
        this->expression=expression;
    }
    R accept(typename Stmt<R>::template Visitor<R>* visitor) override {
        return visitor->visitExpressionStmt(this);
    }
};

template <typename R>
class Function : public Stmt<R> {
public:
    shared_ptr<Token> name;
    vector<shared_ptr<Token>> params;
    vector<shared_ptr<Stmt<R>>> body;
    Function(shared_ptr<Token> name, vector<shared_ptr<Token>> params, vector<shared_ptr<Stmt<R>>> body) {
        this->name=name;
        this->params=params;
        this->body=body;
    }
    R accept(typename Stmt<R>::template Visitor<R>* visitor) override {
        return visitor->visitFunctionStmt(this);
    }
};

template <typename R>
class If : public Stmt<R> {
public:
    shared_ptr<Expr<R>> condition;
    shared_ptr<Stmt<R>> thenBranch;
    shared_ptr<Stmt<R>> elseBranch;
    If(shared_ptr<Expr<R>> condition, shared_ptr<Stmt<R>> thenBranch, shared_ptr<Stmt<R>> elseBranch) {
        this->condition=condition;
        this->thenBranch=thenBranch;
        this->elseBranch=elseBranch;
    }
    R accept(typename Stmt<R>::template Visitor<R>* visitor) override {
        return visitor->visitIfStmt(this);
    }
};

template <typename R>
class Print : public Stmt<R> {
public:
    shared_ptr<Expr<R>> expression;
    Print(shared_ptr<Expr<R>> expression) {
        this->expression=expression;
    }
    R accept(typename Stmt<R>::template Visitor<R>* visitor) override {
        return visitor->visitPrintStmt(this);
    }
};

template <typename R>
class Return : public Stmt<R> {
public:
    shared_ptr<Token> keyword;
    shared_ptr<Expr<R>> value;
    Return(shared_ptr<Token> keyword, shared_ptr<Expr<R>> value) {
        this->keyword=keyword;
        this->value=value;
    }
    R accept(typename Stmt<R>::template Visitor<R>* visitor) override {
        return visitor->visitReturnStmt(this);
    }
};

template <typename R>
class Var : public Stmt<R> {
public:
    shared_ptr<Token> name;
    shared_ptr<Expr<R>> initializer;
    Var(shared_ptr<Token> name, shared_ptr<Expr<R>> initializer) {
        this->name=name;
        this->initializer=initializer;
    }
    R accept(typename Stmt<R>::template Visitor<R>* visitor) override {
        return visitor->visitVarStmt(this);
    }
};

template <typename R>
class While : public Stmt<R> {
public:
    shared_ptr<Expr<R>> condition;
    shared_ptr<Stmt<R>> body;
    While(shared_ptr<Expr<R>> condition, shared_ptr<Stmt<R>> body) {
        this->condition=condition;
        this->body=body;
    }
    R accept(typename Stmt<R>::template Visitor<R>* visitor) override {
        return visitor->visitWhileStmt(this);
    }
};

#endif
