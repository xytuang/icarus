#include <vector>
#include <any>


#include "cleaner.h"

Cleaner::Cleaner(){}

//Clean expression
void Cleaner::clean(Expr<std::any>* expr) {
    expr->accept(this);
}

//Clean statement
void Cleaner::clean(Stmt<std::any>* stmt) {
    stmt->accept(this);
}

void Cleaner::clean(std::vector<Stmt<std::any>*> statements) {
    for (auto stmt : statements) {
        clean(stmt);
    }
}

//Assign expressions
std::any Cleaner::visitAssignExpr(Assign<std::any>* expr) {
    delete expr->name;
    clean(expr->value);
    delete expr->value;

    return nullptr;
}

//Call expressions
std::any Cleaner::visitCallExpr(Call<std::any>* expr) {
    clean(expr->callee);
    delete expr->callee;
    delete expr->paren;
    for (auto arg : expr->arguments) {
        clean(arg);
    }
    return nullptr;
}

//Binary expressions
std::any Cleaner::visitBinaryExpr(Binary<std::any>* expr) {
    clean(expr->left);
    delete expr->left;
    delete expr->operation;
    clean(expr->right);
    delete expr->right;

    return nullptr;
}

//Grouping expressions
std::any Cleaner::visitGroupingExpr(Grouping<std::any>* expr) {
    clean(expr->expression);
    delete expr->expression;

    return nullptr;
}

//Literal expressions
std::any Cleaner::visitLiteralExpr(Literal<std::any>* expr) {
    return nullptr;
}

//Logical expressions
std::any Cleaner::visitLogicalExpr(Logical<std::any>* expr) {
    clean(expr->left);
    delete expr->left;
    clean(expr->right);
    delete expr->right;
    delete expr->operation;

    return nullptr;
}

//Unary expressions
std::any Cleaner::visitUnaryExpr(Unary<std::any>* expr) {
    delete expr->operation;
    clean(expr->right);
    delete expr->right;

    return nullptr;
}

//Variable expressions
std::any Cleaner::visitVariableExpr(Variable<std::any>* expr) {
    delete expr->name;

    return nullptr;
}

//Block statements
std::any Cleaner::visitBlockStmt(Block<std::any>* stmt) {
    clean(stmt->statements);

    return nullptr;
}

//Expression statements
std::any Cleaner::visitExpressionStmt(Expression<std::any>* stmt) {
    clean(stmt->expression);
    delete stmt->expression;

    return nullptr;
}

//Function statements
std::any Cleaner::visitFunctionStmt(Function<std::any>* stmt) {
    delete stmt->name;
    for (auto param : stmt->params) {
        delete param;
    }
    clean(stmt->body);

    return nullptr;
}

//If statements
std::any Cleaner::visitIfStmt(If<std::any>* stmt) {
    clean(stmt->condition);
    delete stmt->condition;
    clean(stmt->thenBranch);
    clean(stmt->elseBranch);

    return nullptr;
}

//Print statements
std::any Cleaner::visitPrintStmt(Print<std::any>* stmt) {
    clean(stmt->expression);
    delete stmt;

    return nullptr;
}

//Return statements
std::any Cleaner::visitReturnStmt(Return<std::any>* stmt) {
    delete stmt->keyword;
    clean(stmt->value);
    delete stmt->value;

    return nullptr;
}

//Var statements
std::any Cleaner::visitVarStmt(Var<std::any>* stmt) {
    delete stmt->name;
    clean(stmt->initializer);
    delete stmt->initializer;

    return nullptr;
}

//While statements
std::any Cleaner::visitWhileStmt(While<std::any>* stmt) {
    clean(stmt->condition);
    delete stmt->condition;
    clean(stmt->body);
    delete stmt->body;

    return nullptr;
}

