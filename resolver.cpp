#include <vector>
#include <unordered_map>
#include <any>
#include <string>

#include "expr.h"
#include "stmt.h"
#include "resolver.h"
#include "interpreter.h"
#include "token.h"
#include "icarus.h"

Resolver::Resolver(Interpreter* interpreter) {
    this->interpreter = interpreter;
}

void Resolver::resolve(Stmt<std::any>* stmt) {
    stmt->accept(this);
}

void Resolver::resolve(Expr<std::any>* expr) {
    expr->accept(this);
}

void Resolver::resolve(std::vector<Stmt<std::any>*> stmts) {
    for (Stmt<std::any>* stmt : stmts) {
        resolve(stmt);
    }
}

void Resolver::resolveFunction(Function<std::any>* function) {
    beginScope();
    for (Token* param : function->params) {
        declare(param);
        define(param);
    }
    resolve(function->body);
    endScope();
}

void Resolver::beginScope() {
    unordered_map<std::string, bool> temp;
    scopes.push_back(temp);
}

void Resolver::endScope() {
    scopes.pop_back();
}

void Resolver::declare(Token* name) {
    if (scopes.size() == 0) return;
    unordered_map<std::string, bool> scope = scopes[scopes.size() - 1];
    scope[name->getLexeme()] = false;
}

void Resolver::define(Token* name) {

    if (scopes.size() == 0) return;
    unordered_map<std::string, bool> scope = scopes[scopes.size() - 1];
    scope[name->getLexeme()] = true;
}

void Resolver::resolveLocal(Expr<std::any>* expr, Token* name) {
    for (int i = scopes.size() - 1; i >= 0; i--) {
        if ((scopes[i].find(name->getLexeme()) != scopes[i].end())) {
            interpreter->resolve(expr, scopes.size() - 1 - i);
            return;
        }
    }
}


//Resolving blocks
std::any Resolver::visitBlockStmt(Block<std::any>* stmt) {
    beginScope();
    resolve(stmt->statements);
    endScope();
    return nullptr;
}


//Variables initialization
std::any Resolver::visitVarStmt(Var<std::any>* stmt) {
    declare(stmt->name);
    if (stmt->initializer) {
        resolve(stmt->initializer);
    }
    define(stmt->name);
    return nullptr;
}

//Variable expressions
std::any Resolver::visitVariableExpr(Variable<std::any>* expr) {
    if (!scopes.empty() && (scopes[scopes.size() - 1][expr->name->getLexeme()] == false)) {
        Icarus::error(expr->name, "Can't read local variable in its own initializer");
    }
    resolveLocal(expr, expr->name);
    return nullptr;
}

//Assign expressions
std::any Resolver::visitAssignExpr(Assign<std::any>* expr) {
    resolve(expr->value);
    resolveLocal(expr, expr->name);
    return nullptr;
}

//Function declarations
std::any Resolver::visitFunctionStmt(Function<std::any>* stmt) {
    declare(stmt->name);
    define(stmt->name);
    resolveFunction(stmt);
    return nullptr;
}

//Expression statements
std::any visitExpressionStmt(Expression<std::any>* stmt) {
    resolve(stmt->expression);
    return nullptr;
}

//If statements
std::any visitIfStmt(If<std::any>* stmt) {
    resolve(stmt->condition);
    resolve(stmt->thenBranch);
    if (stmt->elseBranch) resolve(stmt->elseBranch);
    return nullptr;
}

//Print statements
std::any visitPrintStmt(Print<std::any>* stmt) {
    resolve(stmt->expression);
    return nullptr;
}

//Return statements
std::any visitReturnStmt(Return<std::any>* stmt) {
    if (stmt->value) {
        resolve(stmt->value);
    }
    return nullptr;
}

//While statements
std::any visitWhileStmt(While<std::any>* stmt) {
    resolve(stmt->condition);
    resolve(stmt->body);
    return nullptr;
}

//Binary expressions
std::any visitBinaryExpr(Binary<std::any>* expr) {
    resolve(expr->left);
    resolve(expr->right);
    return nullptr;
}

//Call expressions
std::any visitCallExpr(Call<std::any>* expr) {
    resolve(expr->callee);
    for (Expr<std::any>* argument : expr->arguments) {
        resolve(argument);
    }
    return nullptr;
}

//Grouping expressions
std::any visitGroupingExpr(Grouping<std::any>* expr) {
    resolve(expr->expression);
    return nullptr;
}

//Literals
std::any visitLiteralExpr(Literal<std::any>* expr) {
    return nullptr;
}


//Logical expressions
std::any visitLogicalExpr(Logical<std::any>* expr) {
    resolve(expr->left);
    resolve(expr->right);
    return nullptr;
}

//Unary expressions
std::any visitUnaryExpr(Unary<std::any>* expr) {
    resolve(expr->right);
    return nullptr;
}
