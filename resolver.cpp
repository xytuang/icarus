#include <any>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

#include "token.h"
#include "stmt.h"
#include "resolver.h"
#include "interpreter.h"
#include "icarus.h"
#include "enums.h"

Resolver::Resolver(Interpreter* interpreter) {
    this->interpreter = interpreter;
}

void Resolver::resolve(std::shared_ptr<Expr<std::any>> expr) {
    expr->accept(this);
}

void Resolver::resolve(std::shared_ptr<Stmt<std::any>> stmt) {
    stmt->accept(this);
}


void Resolver::resolve(std::vector<std::shared_ptr<Stmt<std::any>>> statements) {
    for (std::shared_ptr<Stmt<std::any>> stmt : statements) {
        resolve(stmt);
    }
}

void Resolver::declare(std::shared_ptr<Token> name) {
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

void Resolver::define(std::shared_ptr<Token> name) {
    if (this->scopes.size() == 0) {
        return;
    }
    this->scopes.back()[name->getLexeme()] = true;
}

void Resolver::resolveLocal(std::shared_ptr<Expr<std::any>> expr, std::shared_ptr<Token> name) {
    for (int i = this->scopes.size() - 1; i >= 0; i--) {
        if (this->scopes[i].find(name->getLexeme()) != this->scopes[i].end()) {
            this->interpreter->resolve(expr, this->scopes.size() - 1 - i);
            return;
        }
    }
}

void Resolver::resolveFunction(std::shared_ptr<Function<std::any>> function, FunctionType type) {
    // handle rogue return statements
    FunctionType enclosingFunction = this->currentFunction;
    this->currentFunction = type;

    beginScope();
    for (std::shared_ptr<Token> param : function->params) {
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

std::any Resolver::visitBlockStmt(std::shared_ptr<Block<std::any>> stmt) {
    beginScope();
    resolve(stmt->statements);
    endScope();
    return nullptr;
}

std::any Resolver::visitClassStmt(std::shared_ptr<Class<std::any>> stmt) {
    ClassType enclosingClass = this->currentClass;
    this->currentClass = ClassType::CLASS;
    declare(stmt->name);
    define(stmt->name);

    beginScope();
    scopes.back()["this"] = true;
    for (std::shared_ptr<Stmt<std::any>> method : stmt->methods) {
        FunctionType declaration = FunctionType::METHOD;
        std::shared_ptr<Function<std::any>> functionObj = dynamic_pointer_cast<Function<std::any>>(method);
        if (functionObj->name->getLexeme() == "init") {
            declaration = FunctionType::INITIALIZER;
        }
        resolveFunction(dynamic_pointer_cast<Function<std::any>>(method), declaration);
    }
    endScope();
    this->currentClass = enclosingClass;
    return nullptr;
}

std::any Resolver::visitVarStmt(std::shared_ptr<Var<std::any>> stmt) {
    declare(stmt->name);
    if (stmt->initializer != nullptr) {
        resolve(stmt->initializer);
    }
    define(stmt->name);
    return nullptr;
}

std::any Resolver::visitFunctionStmt(std::shared_ptr<Function<std::any>> stmt) {
    declare(stmt->name);
    define(stmt->name);

    resolveFunction(stmt, FunctionType::FUNCTION);
    return nullptr;
}

std::any Resolver::visitExpressionStmt(std::shared_ptr<Expression<std::any>> stmt) {
    resolve(stmt->expression);
    return nullptr;
}

std::any Resolver::visitIfStmt(std::shared_ptr<If<std::any>> stmt) {
    resolve(stmt->condition);
    resolve(stmt->thenBranch);
    if (stmt->elseBranch != nullptr) {
        resolve(stmt->elseBranch);
    }
    return nullptr;
}


std::any Resolver::visitPrintStmt(std::shared_ptr<Print<std::any>> stmt) {
    resolve(stmt->expression);
    return nullptr;
}

std::any Resolver::visitReturnStmt(std::shared_ptr<Return<std::any>> stmt) {
    if (this->currentFunction == FunctionType::NONE) {
        Icarus::error(stmt->keyword, "Can't return from top level code");
    }
    if (stmt->value != nullptr) {
        if (this->currentFunction == FunctionType::INITIALIZER) {
            Icarus::error(stmt->keyword, "Can't return a value from initializer");
        }
        resolve(stmt->value);
    }
    return nullptr;
}

std::any Resolver::visitWhileStmt(std::shared_ptr<While<std::any>> stmt) {
    resolve(stmt->condition);
    resolve(stmt->body);
    return nullptr;
}


std::any Resolver::visitAssignExpr(std::shared_ptr<Assign<std::any>> expr) {
    resolve(expr->value);
    resolveLocal(expr, expr->name);
    return nullptr;
}

std::any Resolver::visitVariableExpr(std::shared_ptr<Variable<std::any>> expr) {
    if (this->scopes.size() != 0  && this->scopes.back()[expr->name->getLexeme()] == false) {
        Icarus::error(expr->name, "Can't read local variable in its own initializer");
    }
    resolveLocal(expr, expr->name);
    return nullptr;
}

std::any Resolver::visitBinaryExpr(std::shared_ptr<Binary<std::any>> expr) {
    resolve(expr->left);
    resolve(expr->right);
    return nullptr;
}

std::any Resolver::visitCallExpr(std::shared_ptr<Call<std::any>> expr) {
    resolve(expr->callee);
    for (std::shared_ptr<Expr<std::any>> arg : expr->arguments) {
        resolve(arg);
    }
    return nullptr;
}

std::any Resolver::visitGetExpr(std::shared_ptr<Get<std::any>> expr) {
    resolve(expr->object);
    return nullptr;
}

std::any Resolver::visitGroupingExpr(std::shared_ptr<Grouping<std::any>> expr) {
    resolve(expr->expression);
    return nullptr;
}


std::any Resolver::visitLiteralExpr(std::shared_ptr<Literal<std::any>> expr) {
    return nullptr;
}


std::any Resolver::visitLogicalExpr(std::shared_ptr<Logical<std::any>> expr) {
    resolve(expr->left);
    resolve(expr->right);
    return nullptr;
}

std::any Resolver::visitSetExpr(std::shared_ptr<Set<std::any>> expr) {
    resolve(expr->value);
    resolve(expr->object);
    return nullptr;
}


std::any Resolver::visitThisExpr(std::shared_ptr<This<std::any>> expr) {
    if (this->currentClass == ClassType::NONE) {
        Icarus::error(expr->keyword, "Can't use \'this\' outside of class");
        return nullptr;
    }
    resolveLocal(expr, expr->keyword);
    return nullptr;
}

std::any Resolver::visitUnaryExpr(std::shared_ptr<Unary<std::any>> expr) {
    resolve(expr->right);
    return nullptr;
}
