#include <any>
#include <unordered_map>
#include <string>
#include <vector>

#include "token.h"
#include "stmt.h"
#include "resolver.h"
#include "interpreter.h"
#include "icarus.h"
#include "enums.h"

Resolver::Resolver(Interpreter* interpreter) {
    this->interpreter = interpreter;
}



void Resolver::resolve(Expr<std::any>* expr) {
    expr->accept(this);
}

void Resolver::resolve(Stmt<std::any>* stmt) {
    stmt->accept(this);
}


void Resolver::resolve(std::vector<Stmt<std::any>*> statements) {
    for (Stmt<std::any>* stmt : statements) {
        resolve(stmt);
    }
}

void Resolver::declare(Token* name) {
    if (this->scopes.size() == 0) {
        return;
    }
    unordered_map<std::string, bool> scope = this->scopes.back();
    if (scope.find(name->getLexeme()) != scope.end()) {
        Icarus::error(name, "Already a variable with this name in this scope");
    }
    scope[name->getLexeme()] = false;
    return;
}

void Resolver::define(Token* name) {
    if (this->scopes.size() == 0) {
        return;
    }
    this->scopes.back()[name->getLexeme()] = true;
}

void Resolver::resolveLocal(Expr<std::any>* expr, Token* name) {
    for (int i = this->scopes.size() - 1; i >= 0; i--) {
        if (this->scopes[i].find(name->getLexeme()) != this->scopes[i].end()) {
            this->interpreter->resolve(expr, this->scopes.size() - 1 - i);
            return;
        }
    }
}

void Resolver::resolveFunction(Function<std::any>* function, FunctionType type) {
    // handle rogue return statements
    FunctionType enclosingFunction = this->currentFunction;
    this->currentFunction = type;

    beginScope();
    for (Token* param : function->params) {
        declare(param);
        define(param);
    }
    resolve(function->body);
    endScope();
    currentFunction = enclosingFunction;
}

void Resolver::beginScope() {
    unordered_map<std::string, bool> map;
    this->scopes.push_back(map);
}


void Resolver::endScope() {
    this->scopes.pop_back();
}

std::any Resolver::visitBlockStmt(Block<std::any>* stmt) {
    beginScope();
    resolve(stmt->statements);
    endScope();
    return nullptr;
}

std::any Resolver::visitClassStmt(Class<std::any>* stmt) {
    declare(stmt->name);
    define(stmt->name);
    return nullptr;
}

std::any Resolver::visitVarStmt(Var<std::any>* stmt) {
    declare(stmt->name);
    if (stmt->initializer != nullptr) {
        resolve(stmt->initializer);
    }
    define(stmt->name);
    return nullptr;
}

std::any Resolver::visitFunctionStmt(Function<std::any>* stmt) {
    declare(stmt->name);
    define(stmt->name);

    resolveFunction(stmt, FUNCTION);
    return nullptr;
}

std::any Resolver::visitExpressionStmt(Expression<std::any>* stmt) {
    resolve(stmt->expression);
    return nullptr;
}

std::any Resolver::visitIfStmt(If<std::any>* stmt) {
    resolve(stmt->condition);
    resolve(stmt->thenBranch);
    if (stmt->elseBranch != nullptr) {
        resolve(stmt->elseBranch);
    }
    return nullptr;
}



std::any Resolver::visitPrintStmt(Print<std::any>* stmt) {
    resolve(stmt->expression);
    return nullptr;
}

std::any Resolver::visitReturnStmt(Return<std::any>* stmt) {
    if (this->currentFunction == NONE) {
        Icarus::error(stmt->keyword, "Can't return from top level code");
    }
    if (stmt->value != nullptr) {
        resolve(stmt->value);
    }
    return nullptr;
}

std::any Resolver::visitWhileStmt(While<std::any>* stmt) {
    resolve(stmt->condition);
    resolve(stmt->body);
    return nullptr;
}


std::any Resolver::visitAssignExpr(Assign<std::any>* expr) {
    resolve(expr->value);
    resolveLocal(expr, expr->name);
    return nullptr;
}

std::any Resolver::visitVariableExpr(Variable<std::any>* expr) {
    if (this->scopes.size() != 0 && this->scopes.back()[expr->name->getLexeme()] == false) {
        Icarus::error(expr->name, "Can't read local variable in its own initializer");
    }
    resolveLocal(expr, expr->name);
    return nullptr;
}

std::any Resolver::visitBinaryExpr(Binary<std::any>* expr) {
    resolve(expr->left);
    resolve(expr->right);
    return nullptr;
}

std::any Resolver::visitCallExpr(Call<std::any>* expr) {
    resolve(expr->callee);
    for (Expr<std::any>* arg : expr->arguments) {
        resolve(arg);
    }
    return nullptr;
}


std::any Resolver::visitGroupingExpr(Grouping<std::any>* expr) {
    resolve(expr->expression);
    return nullptr;
}


std::any Resolver::visitLiteralExpr(Literal<std::any>* expr) {
    return nullptr;
}


std::any Resolver::visitLogicalExpr(Logical<std::any>* expr) {
    resolve(expr->left);
    resolve(expr->right);
    return nullptr;
}


std::any Resolver::visitUnaryExpr(Unary<std::any>* expr) {
    resolve(expr->right);
    return nullptr;
}
