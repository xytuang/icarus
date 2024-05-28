#ifndef EXPR_H
#define EXPR_H
#include <vector>
#include <string>
#include <any>
#include "token.h"
using namespace std;

template <typename R> class Assign;
template <typename R> class Binary;
template <typename R> class Grouping;
template <typename R> class Literal;
template <typename R> class Logical;
template <typename R> class Unary;
template <typename R> class Variable;

template <typename R>
class Expr{
public:
    template <typename T>
    class Visitor {
    public:
        virtual T visitAssignExpr (Assign<R>* expr) = 0;
        virtual T visitBinaryExpr (Binary<R>* expr) = 0;
        virtual T visitGroupingExpr (Grouping<R>* expr) = 0;
        virtual T visitLiteralExpr (Literal<R>* expr) = 0;
        virtual T visitLogicalExpr (Logical<R>* expr) = 0;
        virtual T visitUnaryExpr (Unary<R>* expr) = 0;
        virtual T visitVariableExpr (Variable<R>* expr) = 0;
        virtual ~Visitor() = default;
    };

    virtual R accept(Visitor<R>* visitor) = 0;
    virtual ~Expr() = default;
};

template <typename R>
class Assign : public Expr<R> {
public:
    Token* name;
    Expr<R>* value;
    Assign(Token* name, Expr<R>* value) {
        this->name=name;
        this->value=value;
    }
    R accept(typename Expr<R>::template Visitor<R>* visitor) override {
        return visitor->visitAssignExpr(this);
    }
};

template <typename R>
class Binary : public Expr<R> {
public:
    Expr<R>* left;
    Token* operation;
    Expr<R>* right;
    Binary(Expr<R>* left, Token* operation, Expr<R>* right) {
        this->left=left;
        this->operation=operation;
        this->right=right;
    }
    R accept(typename Expr<R>::template Visitor<R>* visitor) override {
        return visitor->visitBinaryExpr(this);
    }
};

template <typename R>
class Grouping : public Expr<R> {
public:
    Expr<R>* expression;
    Grouping(Expr<R>* expression) {
        this->expression=expression;
    }
    R accept(typename Expr<R>::template Visitor<R>* visitor) override {
        return visitor->visitGroupingExpr(this);
    }
};

template <typename R>
class Literal : public Expr<R> {
public:
    any value;
    Literal(any value) {
        this->value=value;
    }
    R accept(typename Expr<R>::template Visitor<R>* visitor) override {
        return visitor->visitLiteralExpr(this);
    }
};

template <typename R>
class Logical : public Expr<R> {
public:
    Expr<R>* left;
    Token* operation;
    Expr<R>* right;
    Logical(Expr<R>* left, Token* operation, Expr<R>* right) {
        this->left=left;
        this->operation=operation;
        this->right=right;
    }
    R accept(typename Expr<R>::template Visitor<R>* visitor) override {
        return visitor->visitLogicalExpr(this);
    }
};

template <typename R>
class Unary : public Expr<R> {
public:
    Token* operation;
    Expr<R>* right;
    Unary(Token* operation, Expr<R>* right) {
        this->operation=operation;
        this->right=right;
    }
    R accept(typename Expr<R>::template Visitor<R>* visitor) override {
        return visitor->visitUnaryExpr(this);
    }
};

template <typename R>
class Variable : public Expr<R> {
public:
    Token* name;
    Variable(Token* name) {
        this->name=name;
    }
    R accept(typename Expr<R>::template Visitor<R>* visitor) override {
        return visitor->visitVariableExpr(this);
    }
};

#endif
