#include <any>
#include <stdexcept>
#include <iostream>
#include "interpreter.h"
#include "expr.h"
#include "token.h"
#include "tokentype.h"
#include "runtime_error.h"
#include "icarus.h"

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

void Interpreter::checkNumberOperand(Token* operation, std::any operand) {
    if (operand.type() == typeid(double)) {
        return;
    }
    throw new RuntimeError(operation, "Operand must be a number");
}

void Interpreter::checkNumberOperands(Token* operation, std::any left, std::any right) {

    if (left.type() == typeid(double) && right.type() == typeid(double)) {
        return;
    }
    throw new RuntimeError(operation, "Operands must be numbers");
}


std::string Interpreter::stringify(std::any object) {
    if (object.type() == typeid(nullptr_t)){
        return "nil";
    }

    if (object.type() == typeid(double)){
        double num = std::any_cast<double>(object);
        std::string text = std::to_string(num);
        if (text.substr(text.size() - 2, 2) == ".0") {
            text.erase(text.size() - 2);
        }
        return text;
    }

    else if (object.type() == typeid(std::string)) {
        return std::any_cast<std::string>(object);
    }

    else if(object.type() == typeid(bool)) {
        bool value = std::any_cast<bool>(object);
        return std::to_string(value);
    }
    return "unsupported";
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
            checkNumberOperand(expr->operation, right);
            if (right.type() == typeid(double))
                return -(std::any_cast<double>(right));
            break;
        default:
            return nullptr;
    }
    return nullptr;
}

std::any Interpreter::visitBinaryExpr(Binary<std::any>* expr){
    std::any left = evaluate(expr->left);
    std::any right = evaluate(expr->right);

    switch(expr->operation->getType()) {
        case GREATER:
        {
            checkNumberOperands(expr->operation, left, right);
            return std::any_cast<double>(left) > std::any_cast<double>(right);
        }
            break;

        case GREATER_EQUAL: 
        {
            checkNumberOperands(expr->operation, left, right);
            return std::any_cast<double>(left) >= std::any_cast<double>(right);
        }
            break;
        case LESS:
        {
            checkNumberOperands(expr->operation, left, right);
            return std::any_cast<double>(left) < std::any_cast<double>(right);
        }
            break;

        case LESS_EQUAL:
        {
            checkNumberOperands(expr->operation, left, right);
            return std::any_cast<double>(left) <= std::any_cast<double>(right);
        }
            break;
        case MINUS:

        {
            checkNumberOperands(expr->operation, left, right);
            return std::any_cast<double>(left) - std::any_cast<double>(right);
        }
            break;

        case PLUS:
        { 
            if ((left.type() == typeid(double)) && right.type() == typeid(double)) {
                double leftNum = std::any_cast<double>(left);
                double rightNum = std::any_cast<double>(right);
                return leftNum + rightNum;
            }
            else if (left.type() == typeid(std::string) && right.type() == typeid(std::string)) {
                return std::any_cast<std::string>(left)+(std::any_cast<std::string>(right));
            }

            throw new RuntimeError(expr->operation, "Operands must be two numbers or two strings");
        }
            break;

        case SLASH:
        {
            checkNumberOperands(expr->operation, left, right);
            return std::any_cast<double>(left) / std::any_cast<double>(right);
        }
            break;

        case STAR:
        {
            checkNumberOperands(expr->operation, left, right);

            return std::any_cast<double>(left) * std::any_cast<double>(right);
        }
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

void Interpreter::interpret(Expr<std::any>* expr) {
    try {
        std::any value = evaluate(expr);
        std::cout << stringify(value) << endl;
    } catch (RuntimeError* error){
        Icarus::runtimeError(error);
    }
}
