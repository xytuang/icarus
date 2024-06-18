#include <any>
#include <stdexcept>
#include <iostream>
#include <vector>

#include "interpreter.h"
#include "env.h"
#include "expr.h"
#include "token.h"
#include "tokentype.h"
#include "runtime_error.h"
#include "icarus.h"
#include "icarus_callable.h"
#include "icarus_function.h"
#include "stack_unwinder.h"

Interpreter::Interpreter() {
    this->globals = new Environment();
    this->env = globals;
}

std::any Interpreter::evaluate(Expr<std::any>* expr) {
    return expr->accept(this);
}

std::any Interpreter::execute(Stmt<std::any>* stmt) {
    stmt->accept(this);
    return nullptr;
}

void Interpreter::resolve(Expr<std::any>* expr, int depth) {
    locals[expr] = depth;
}

std::any Interpreter::lookUpVariable(Token* name, Expr<std::any>* expr) {
    if (locals.find(expr) != locals.end()) {
        int distance = locals[expr];
        return env->getAt(distance, name->getLexeme());
    }
    else {
        return globals->get(name);
    }
    
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

    // check the actual values based on the type
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

std::any Interpreter::executeBlock(std::vector<Stmt<std::any>*> statements, Environment* environment) {
    Environment* previous = this->env;
    this->env = environment;

    for (Stmt<std::any>* statement : statements) {
        execute(statement);
    }
    this->env = previous;
    return nullptr;
}


//Assign expressions
std::any Interpreter::visitAssignExpr(Assign<std::any>* expr) {
    std::any value = evaluate(expr->value);
    if (locals.find(expr) != locals.end()) {
        int distance = locals[expr];
        env->assignAt(distance, expr->name, value);
    }
    else {
        globals->assign(expr->name, value);
    }
    return value;
}

//Call expressions
std::any Interpreter::visitCallExpr(Call<std::any>* expr) {
    std::any callee = evaluate(expr->callee);
    std::vector<std::any> arguments;
    for (int i = 0; i < expr->arguments.size(); i++) {
        arguments.push_back(evaluate(expr->arguments[i]));
    }
    IcarusCallable* function;
    try {
        IcarusFunction<std::any>* functionPtr = std::any_cast<IcarusFunction<std::any>*>(callee);
        function = dynamic_cast<IcarusCallable*>(functionPtr);
        /*
        function = std::any_cast<IcarusFunction*>(callee);
        */
    } catch (const std::bad_any_cast& e) {
        throw new RuntimeError(expr->paren, "Can only call functions and classes");
    }
    if (arguments.size() != function->arity()) {
        throw new RuntimeError(expr->paren, "Expected " + std::to_string(function->arity()) + " arguments but got " + std::to_string(arguments.size()));
    }
    return function->call(this, arguments);
}


//Binary Expressions
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


//Grouping expressions
std::any Interpreter::visitGroupingExpr(Grouping<std::any>* expr){
    return evaluate(expr->expression);
}


//Literal expressions
std::any Interpreter::visitLiteralExpr(Literal<std::any>* expr){
    return expr->value;
}

//Logical Expressions
std::any Interpreter::visitLogicalExpr(Logical<std::any>* expr) {
    std::any left = evaluate(expr->left);
    if (expr->operation->getType() == OR) {
        if (isTruthy(left)) { //short circuit and return true given that left was true
            return left;
        }
    }
    else {
        if (!isTruthy(left)) { //short circuit given 'AND' operation and left was false
            return left;
        }
    }
    return evaluate(expr->right);
}


//Unary expressions
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

//variable expressions
std::any Interpreter::visitVariableExpr(Variable<std::any>* expr) {
    return lookUpVariable(expr->name, expr);
}

//STATEMENTS

std::any Interpreter::visitBlockStmt(Block<std::any>* stmt) {
    executeBlock(stmt->statements, new Environment(this->env));
    return nullptr;
}


//expression statements
std::any Interpreter::visitExpressionStmt(Expression<std::any>* stmt) {
    evaluate(stmt->expression);
    return nullptr;
}

//function statements
std::any Interpreter::visitFunctionStmt(Function<std::any>* stmt) {
    IcarusFunction<std::any>* function = new IcarusFunction<std::any>(stmt, this->env);
    this->env->define(stmt->name->getLexeme(), function);
    return nullptr;
}

//if statements
std::any Interpreter::visitIfStmt(If<std::any>* stmt) {
    if (isTruthy(evaluate(stmt->condition))) {
        execute(stmt->thenBranch);
    }
    else {
        execute(stmt->elseBranch);
    }
    return nullptr;
}

//Print statements
std::any Interpreter::visitPrintStmt(Print<std::any>* stmt) {
    std::any value = evaluate(stmt->expression);
    std::cout << stringify(value) << std::endl;
    return nullptr;
}

//Return statements
std::any Interpreter::visitReturnStmt(Return<std::any>* stmt) {
    std::any value = nullptr;
    if (stmt->value != nullptr) {
        value = evaluate(stmt->value);
    }
    throw new StackUnwinder(value);
}

//Var statements
std::any Interpreter::visitVarStmt(Var<std::any>* stmt) {
    std::any value = nullptr;
    if (stmt->initializer != nullptr) {
        value = evaluate(stmt->initializer);
    }
    env->define(stmt->name->getLexeme(), value);
    return nullptr;
}

//While statements
std::any Interpreter::visitWhileStmt(While<std::any>* stmt) {
    while (isTruthy(evaluate(stmt->condition))) {
        execute(stmt->body);
    }
    return nullptr;
}

//interprete statements
std::any Interpreter::interpret(std::vector<Stmt<std::any>*> statements) {
    try {
        for (Stmt<std::any>* stmt : statements) {
            execute(stmt);
        }
    } catch (RuntimeError* error){
        Icarus::runtimeError(error);
    }
    return nullptr;
}
