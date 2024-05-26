#include <any>
#include "interpreter.h"
#include "expr.h"
#include "tokentype.h"

std::any Interpreter::evaluate(Expr<std::any>* expr) {
    return expr->accept(this);
}

bool Interpreter::isEqual(std::any a, std::any b) {
    if (a.type() == typeid(std::nullptr_t) && b.type() == typeid(std::nullptr_t)) {
        return true;
    }
    if (a.type() == typeid(std::nullptr_t)) {
        return false;
    }
    
    if (a.type() != b.type()) {
        return false;
    }

    // Then, check the actual values based on the type
    if (a.type() == typeid(int)) {
        return std::any_cast<int>(a) == std::any_cast<int>(b);
    } else if (a.type() == typeid(double)) {
        return std::any_cast<double>(a) == std::any_cast<double>(b);
    } else if (a.type() == typeid(bool)) {
        return std::any_cast<bool>(a) == std::any_cast<bool>(b);
    } else if (a.type() == typeid(std::string)) {
        return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
    }
    return false;
}

bool Interpreter::isTruthy(std::any object) {
    if (object.type() == typeid(std::nullptr_t)){
        return false;
    }
    if (object.type() == typeid(bool)){
        return std::any_cast<bool>(object);
    }
    return true;

}


std::any Interpreter::visitLiteralExpr(Literal<std::any>* expr){
    return expr->value;
}

std::any Interpreter::visitGroupingExpr(Grouping<std::any>* expr){
    return evaluate(expr->expression);
}

std::any Interpreter::visitUnaryExpr(Unary<std::any>* expr){
    std::any right = evaluate(expr->right);
    switch(expr->operation->getType()) {
        case BANG:
            return isTruthy(right);
        case MINUS:
            if (right.type() == typeid(int))
                return -(std::any_cast<int>(right));
            if (right.type() == typeid(double))
                return -(std::any_cast<double>(right));
            break;
        default:
            return nullptr;
    }
    return nullptr;
}

std::any Interpreter::visitBinaryExpr(Binary<std::any>* expr){
    std::any left = expr->left;
    std::any right = expr->right;

    switch(expr->operation->getType()) {
        case GREATER:
            if (left.type() == typeid(int) && right.type() == typeid(int))
                return std::any_cast<int>(left) > (std::any_cast<int>(right));

            if (left.type() == typeid(double) && right.type() == typeid(double))
                return std::any_cast<double>(left) > (std::any_cast<double>(right));
            break;

        case GREATER_EQUAL:
            if (left.type() == typeid(int) && right.type() == typeid(int))
                return std::any_cast<int>(left) >= (std::any_cast<int>(right));

            if (left.type() == typeid(double) && right.type() == typeid(double))
                return std::any_cast<double>(left) >= (std::any_cast<double>(right));
            break;

        case LESS:
            if (left.type() == typeid(int) && right.type() == typeid(int))
                return std::any_cast<int>(left) < (std::any_cast<int>(right));

            if (left.type() == typeid(double) && right.type() == typeid(double))
                return std::any_cast<double>(left) < (std::any_cast<double>(right));
            break;

        case LESS_EQUAL:
            if (left.type() == typeid(int) && right.type() == typeid(int))
                return std::any_cast<int>(left) <= (std::any_cast<int>(right));

            if (left.type() == typeid(double) && right.type() == typeid(double))
                return std::any_cast<double>(left) <= (std::any_cast<double>(right));
            break;

        case MINUS:
            if (left.type() == typeid(int) && right.type() == typeid(int))
                return std::any_cast<int>(left)-(std::any_cast<int>(right));

            if (left.type() == typeid(double) && right.type() == typeid(double))
                return std::any_cast<double>(left)-(std::any_cast<double>(right));
            break;

        case PLUS:
            if (left.type() == typeid(int) && right.type() == typeid(int))
                return std::any_cast<int>(left)+(std::any_cast<int>(right));

            if (left.type() == typeid(double) && right.type() == typeid(double))
                return std::any_cast<double>(left)+(std::any_cast<double>(right)); 

            if (left.type() == typeid(std::string) && right.type() == typeid(std::string))
                return std::any_cast<std::string>(left)+(std::any_cast<std::string>(right));
            break;

        case SLASH:
            if (left.type() == typeid(int) && right.type() == typeid(int))
                return std::any_cast<int>(left)/(std::any_cast<int>(right));

            if (left.type() == typeid(double) && right.type() == typeid(double))
                return std::any_cast<double>(left)/(std::any_cast<double>(right));
            break;

        case STAR:
            if (left.type() == typeid(int) && right.type() == typeid(int))
                return std::any_cast<int>(left)*(std::any_cast<int>(right));

            if (left.type() == typeid(double) && right.type() == typeid(double))
                return std::any_cast<double>(left)*(std::any_cast<double>(right));
            break;
        case BANG_EQUAL:
            return !isEqual(left, right);

        case EQUAL_EQUAL:
            return isEqual(left, right);
        default:
            return nullptr;
    }
    return nullptr;
}
