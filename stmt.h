#ifndef STMT_H
#define STMT_H
#include <vector>
#include <string>
#include <any>
#include "token.h"
#include "expr.h"

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
    vector<Stmt<R>*> statements;
    Block(vector<Stmt<R>*> statements) {
        this->statements=statements;
    }
    R accept(typename Stmt<R>::template Visitor<R>* visitor) override {
        return visitor->visitBlockStmt(this);
    }
};

template <typename R>
class Class : public Stmt<R> {
public:
    Token* name;
    vector<Stmt<R>*> methods;
    Class(Token* name, vector<Stmt<R>*> methods) {
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
    Expr<R>* expression;
    Expression(Expr<R>* expression) {
        this->expression=expression;
    }
    R accept(typename Stmt<R>::template Visitor<R>* visitor) override {
        return visitor->visitExpressionStmt(this);
    }
};

template <typename R>
class Function : public Stmt<R> {
public:
    Token* name;
    vector<Token*> params;
    vector<Stmt<R>*> body;
    Function(Token* name, vector<Token*> params, vector<Stmt<R>*> body) {
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
    Expr<R>* condition;
    Stmt<R>* thenBranch;
    Stmt<R>* elseBranch;
    If(Expr<R>* condition, Stmt<R>* thenBranch, Stmt<R>* elseBranch) {
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
    Expr<R>* expression;
    Print(Expr<R>* expression) {
        this->expression=expression;
    }
    R accept(typename Stmt<R>::template Visitor<R>* visitor) override {
        return visitor->visitPrintStmt(this);
    }
};

template <typename R>
class Return : public Stmt<R> {
public:
    Token* keyword;
    Expr<R>* value;
    Return(Token* keyword, Expr<R>* value) {
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
    Token* name;
    Expr<R>* initializer;
    Var(Token* name, Expr<R>* initializer) {
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
    Expr<R>* condition;
    Stmt<R>* body;
    While(Expr<R>* condition, Stmt<R>* body) {
        this->condition=condition;
        this->body=body;
    }
    R accept(typename Stmt<R>::template Visitor<R>* visitor) override {
        return visitor->visitWhileStmt(this);
    }
};

#endif
