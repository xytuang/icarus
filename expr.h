#ifndef EXPR_H
#define EXPR_H
#include <vector>
#include <string>
#include <any>
#include "token.h"
using namespace std;

class Binary;
class Grouping;
class Literal;
class Unary;

class Expr{
public:
    class Visitor {
    public:
        virtual string visitBinaryExpr (Binary* expr) = 0;
        virtual string visitGroupingExpr (Grouping* expr) = 0;
        virtual string visitLiteralExpr (Literal* expr) = 0;
        virtual string visitUnaryExpr (Unary* expr) = 0;
        virtual ~Visitor() = default;
    };

    virtual string accept(Visitor* visitor) = 0;
};

class Binary : public Expr {
public:
    Expr* left;
    Token* operation;
    Expr* right;
    Binary(Expr* left, Token* operation, Expr* right) {
        this->left=left;
        this->operation=operation;
        this->right=right;
    }
    string accept(Visitor* visitor) {
        return visitor->visitBinaryExpr(this);
    }
};

class Grouping : public Expr {
public:
    Expr* expression;
    Grouping(Expr* expression) {
        this->expression=expression;
    }
    string accept(Visitor* visitor) {
        return visitor->visitGroupingExpr(this);
    }
};

class Literal : public Expr {
public:
    any value;
    Literal(any value) {
        this->value=value;
    }
    string accept(Visitor* visitor) {
        return visitor->visitLiteralExpr(this);
    }
};

class Unary : public Expr {
public:
    Token* operation;
    Expr* right;
    Unary(Token* operation, Expr* right) {
        this->operation=operation;
        this->right=right;
    }
    string accept(Visitor* visitor) {
        return visitor->visitUnaryExpr(this);
    }
};

#endif
