#include <vector>
#include <string>
#include "token.h"
using namespace std;

template typename<R>
class Visitor {
public:
    virtual R visitBinaryExpr (Binary& expr) = 0;
    virtual R visitGroupingExpr (Grouping& expr) = 0;
    virtual R visitLiteralExpr (Literal& expr) = 0;
    virtual R visitUnaryExpr (Unary& expr) = 0;
    virtual ~Visitor() = default;
};

template typename<R>
class Expr{
public:
    virtual R accept(Visitor<R>& visitor) = 0;
};

class Binary : public Expr {
public:
    Expr left;
    Token operator;
    Expr right;
    Binary(Expr left, Token operator, Expr right) {
        this->left=left;
        this->operator=operator;
        this->right=right;
    }
    template<typename R>
    R accept(Visitor<R>& visitor) override {
        return visitor->visitBinaryExpr(this);
    }
};

class Grouping : public Expr {
public:
    Expr expression;
    Grouping(Expr expression) {
        this->expression=expression;
    }
    template<typename R>
    R accept(Visitor<R>& visitor) override {
        return visitor->visitGroupingExpr(this);
    }
};

class Literal : public Expr {
public:
    string value;
    Literal(string value) {
        this->value=value;
    }
    template<typename R>
    R accept(Visitor<R>& visitor) override {
        return visitor->visitLiteralExpr(this);
    }
};

class Unary : public Expr {
public:
    Token operator;
    Expr right;
    Unary(Token operator, Expr right) {
        this->operator=operator;
        this->right=right;
    }
    template<typename R>
    R accept(Visitor<R>& visitor) override {
        return visitor->visitUnaryExpr(this);
    }
};

