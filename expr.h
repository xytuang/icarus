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
        virtual T visitAssignExpr (shared_ptr<Assign<R>> expr) = 0;
        virtual T visitBinaryExpr (shared_ptr<Binary<R>> expr) = 0;
        virtual T visitCallExpr (shared_ptr<Call<R>> expr) = 0;
        virtual T visitGetExpr (shared_ptr<Get<R>> expr) = 0;
        virtual T visitGroupingExpr (shared_ptr<Grouping<R>> expr) = 0;
        virtual T visitLiteralExpr (shared_ptr<Literal<R>> expr) = 0;
        virtual T visitLogicalExpr (shared_ptr<Logical<R>> expr) = 0;
        virtual T visitSetExpr (shared_ptr<Set<R>> expr) = 0;
        virtual T visitThisExpr (shared_ptr<This<R>> expr) = 0;
        virtual T visitUnaryExpr (shared_ptr<Unary<R>> expr) = 0;
        virtual T visitVariableExpr (shared_ptr<Variable<R>> expr) = 0;
        virtual ~Visitor() = default;
    };

    virtual R accept(Visitor<R>* visitor) = 0;
    virtual ~Expr() = default;
};

template <typename R>
class Assign : public Expr<R>, public enable_shared_from_this<Assign<R>> {
public:
    shared_ptr<Token> name;
    shared_ptr<Expr<R>> value;
    Assign(shared_ptr<Token> name, shared_ptr<Expr<R>> value) {
        this->name=name;
        this->value=value;
    }
    shared_ptr<Assign<R>> getSharedPtr() {
        return this->shared_from_this();
    }
    R accept(typename Expr<R>::template Visitor<R>* visitor) override {
        return visitor->visitAssignExpr(getSharedPtr());
    }
};

template <typename R>
class Binary : public Expr<R>, public enable_shared_from_this<Binary<R>> {
public:
    shared_ptr<Expr<R>> left;
    shared_ptr<Token> operation;
    shared_ptr<Expr<R>> right;
    Binary(shared_ptr<Expr<R>> left, shared_ptr<Token> operation, shared_ptr<Expr<R>> right) {
        this->left=left;
        this->operation=operation;
        this->right=right;
    }
    shared_ptr<Binary<R>> getSharedPtr() {
        return this->shared_from_this();
    }
    R accept(typename Expr<R>::template Visitor<R>* visitor) override {
        return visitor->visitBinaryExpr(getSharedPtr());
    }
};

template <typename R>
class Call : public Expr<R>, public enable_shared_from_this<Call<R>> {
public:
    shared_ptr<Expr<R>> callee;
    shared_ptr<Token> paren;
    vector<shared_ptr<Expr<R>>> arguments;
    Call(shared_ptr<Expr<R>> callee, shared_ptr<Token> paren, vector<shared_ptr<Expr<R>>> arguments) {
        this->callee=callee;
        this->paren=paren;
        this->arguments=arguments;
    }
    shared_ptr<Call<R>> getSharedPtr() {
        return this->shared_from_this();
    }
    R accept(typename Expr<R>::template Visitor<R>* visitor) override {
        return visitor->visitCallExpr(getSharedPtr());
    }
};

template <typename R>
class Get : public Expr<R>, public enable_shared_from_this<Get<R>> {
public:
    shared_ptr<Expr<R>> object;
    shared_ptr<Token> name;
    Get(shared_ptr<Expr<R>> object, shared_ptr<Token> name) {
        this->object=object;
        this->name=name;
    }
    shared_ptr<Get<R>> getSharedPtr() {
        return this->shared_from_this();
    }
    R accept(typename Expr<R>::template Visitor<R>* visitor) override {
        return visitor->visitGetExpr(getSharedPtr());
    }
};

template <typename R>
class Grouping : public Expr<R>, public enable_shared_from_this<Grouping<R>> {
public:
    shared_ptr<Expr<R>> expression;
    Grouping(shared_ptr<Expr<R>> expression) {
        this->expression=expression;
    }
    shared_ptr<Grouping<R>> getSharedPtr() {
        return this->shared_from_this();
    }
    R accept(typename Expr<R>::template Visitor<R>* visitor) override {
        return visitor->visitGroupingExpr(getSharedPtr());
    }
};

template <typename R>
class Literal : public Expr<R>, public enable_shared_from_this<Literal<R>> {
public:
    any value;
    Literal(any value) {
        this->value=value;
    }
    shared_ptr<Literal<R>> getSharedPtr() {
        return this->shared_from_this();
    }
    R accept(typename Expr<R>::template Visitor<R>* visitor) override {
        return visitor->visitLiteralExpr(getSharedPtr());
    }
};

template <typename R>
class Logical : public Expr<R>, public enable_shared_from_this<Logical<R>> {
public:
    shared_ptr<Expr<R>> left;
    shared_ptr<Token> operation;
    shared_ptr<Expr<R>> right;
    Logical(shared_ptr<Expr<R>> left, shared_ptr<Token> operation, shared_ptr<Expr<R>> right) {
        this->left=left;
        this->operation=operation;
        this->right=right;
    }
    shared_ptr<Logical<R>> getSharedPtr() {
        return this->shared_from_this();
    }
    R accept(typename Expr<R>::template Visitor<R>* visitor) override {
        return visitor->visitLogicalExpr(getSharedPtr());
    }
};

template <typename R>
class Set : public Expr<R>, public enable_shared_from_this<Set<R>> {
public:
    shared_ptr<Expr<R>> object;
    shared_ptr<Token> name;
    shared_ptr<Expr<R>> value;
    Set(shared_ptr<Expr<R>> object, shared_ptr<Token> name, shared_ptr<Expr<R>> value) {
        this->object=object;
        this->name=name;
        this->value=value;
    }
    shared_ptr<Set<R>> getSharedPtr() {
        return this->shared_from_this();
    }
    R accept(typename Expr<R>::template Visitor<R>* visitor) override {
        return visitor->visitSetExpr(getSharedPtr());
    }
};

template <typename R>
class This : public Expr<R>, public enable_shared_from_this<This<R>> {
public:
    shared_ptr<Token> keyword;
    This(shared_ptr<Token> keyword) {
        this->keyword=keyword;
    }
    shared_ptr<This<R>> getSharedPtr() {
        return this->shared_from_this();
    }
    R accept(typename Expr<R>::template Visitor<R>* visitor) override {
        return visitor->visitThisExpr(getSharedPtr());
    }
};

template <typename R>
class Unary : public Expr<R>, public enable_shared_from_this<Unary<R>> {
public:
    shared_ptr<Token> operation;
    shared_ptr<Expr<R>> right;
    Unary(shared_ptr<Token> operation, shared_ptr<Expr<R>> right) {
        this->operation=operation;
        this->right=right;
    }
    shared_ptr<Unary<R>> getSharedPtr() {
        return this->shared_from_this();
    }
    R accept(typename Expr<R>::template Visitor<R>* visitor) override {
        return visitor->visitUnaryExpr(getSharedPtr());
    }
};

template <typename R>
class Variable : public Expr<R>, public enable_shared_from_this<Variable<R>> {
public:
    shared_ptr<Token> name;
    Variable(shared_ptr<Token> name) {
        this->name=name;
    }
    shared_ptr<Variable<R>> getSharedPtr() {
        return this->shared_from_this();
    }
    R accept(typename Expr<R>::template Visitor<R>* visitor) override {
        return visitor->visitVariableExpr(getSharedPtr());
    }
};

#endif
