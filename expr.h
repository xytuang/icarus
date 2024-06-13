#ifndef EXPR_H
#define EXPR_H
#include <vector>
#include <string>
#include <any>
#include <memory>
#include "token.h"
using namespace std;

template <typename R> class Assign;
template <typename R> class Binary;
template <typename R> class Call;
template <typename R> class Get;
template <typename R> class Grouping;
template <typename R> class Literal;
template <typename R> class Logical;
template <typename R> class Set;
template <typename R> class This;
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
        virtual T visitCallExpr (Call<R>* expr) = 0;
        virtual T visitGetExpr (Get<R>* expr) = 0;
        virtual T visitGroupingExpr (Grouping<R>* expr) = 0;
        virtual T visitLiteralExpr (Literal<R>* expr) = 0;
        virtual T visitLogicalExpr (Logical<R>* expr) = 0;
        virtual T visitSetExpr (Set<R>* expr) = 0;
        virtual T visitThisExpr (This<R>* expr) = 0;
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
    shared_ptr<Token> name;
    shared_ptr<Expr<R>> value;
    Assign(shared_ptr<Token> name, shared_ptr<Expr<R>> value) {
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
    shared_ptr<Expr<R>> left;
    shared_ptr<Token> operation;
    shared_ptr<Expr<R>> right;
    Binary(shared_ptr<Expr<R>> left, shared_ptr<Token> operation, shared_ptr<Expr<R>> right) {
        this->left=left;
        this->operation=operation;
        this->right=right;
    }
    R accept(typename Expr<R>::template Visitor<R>* visitor) override {
        return visitor->visitBinaryExpr(this);
    }
};

template <typename R>
class Call : public Expr<R> {
public:
    shared_ptr<Expr<R>> callee;
    shared_ptr<Token> paren;
    vector<shared_ptr<Expr<R>>> arguments;
    Call(shared_ptr<Expr<R>> callee, shared_ptr<Token> paren, vector<shared_ptr<Expr<R>>> arguments) {
        this->callee=callee;
        this->paren=paren;
        this->arguments=arguments;
    }
    R accept(typename Expr<R>::template Visitor<R>* visitor) override {
        return visitor->visitCallExpr(this);
    }
};

template <typename R>
class Get : public Expr<R> {
public:
    shared_ptr<Expr<R>> object;
    shared_ptr<Token> name;
    Get(shared_ptr<Expr<R>> object, shared_ptr<Token> name) {
        this->object=object;
        this->name=name;
    }
    R accept(typename Expr<R>::template Visitor<R>* visitor) override {
        return visitor->visitGetExpr(this);
    }
};

template <typename R>
class Grouping : public Expr<R> {
public:
    shared_ptr<Expr<R>> expression;
    Grouping(shared_ptr<Expr<R>> expression) {
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
    shared_ptr<Expr<R>> left;
    shared_ptr<Token> operation;
    shared_ptr<Expr<R>> right;
    Logical(shared_ptr<Expr<R>> left, shared_ptr<Token> operation, shared_ptr<Expr<R>> right) {
        this->left=left;
        this->operation=operation;
        this->right=right;
    }
    R accept(typename Expr<R>::template Visitor<R>* visitor) override {
        return visitor->visitLogicalExpr(this);
    }
};

template <typename R>
class Set : public Expr<R> {
public:
    shared_ptr<Expr<R>> object;
    shared_ptr<Token> name;
    shared_ptr<Expr<R>> value;
    Set(shared_ptr<Expr<R>> object, shared_ptr<Token> name, shared_ptr<Expr<R>> value) {
        this->object=object;
        this->name=name;
        this->value=value;
    }
    R accept(typename Expr<R>::template Visitor<R>* visitor) override {
        return visitor->visitSetExpr(this);
    }
};

template <typename R>
class This : public Expr<R> {
public:
    shared_ptr<Token> keyword;
    This(shared_ptr<Token> keyword) {
        this->keyword=keyword;
    }
    R accept(typename Expr<R>::template Visitor<R>* visitor) override {
        return visitor->visitThisExpr(this);
    }
};

template <typename R>
class Unary : public Expr<R> {
public:
    shared_ptr<Token> operation;
    shared_ptr<Expr<R>> right;
    Unary(shared_ptr<Token> operation, shared_ptr<Expr<R>> right) {
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
    shared_ptr<Token> name;
    Variable(shared_ptr<Token> name) {
        this->name=name;
    }
    R accept(typename Expr<R>::template Visitor<R>* visitor) override {
        return visitor->visitVariableExpr(this);
    }
};

#endif
