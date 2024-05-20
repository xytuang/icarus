#include <vector>

class Expr{
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
};

class Grouping : public Expr {
public:
    Expr expression;
    Grouping(Expr expression) {
        this->expression=expression;
    }
};

class Literal : public Expr {
public:
    Object value;
    Literal(Object value) {
        this->value=value;
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
};

