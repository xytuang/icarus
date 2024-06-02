#include <any>
#include <unordered_map>
#include <string>
#include <vector>

#include "stmt.h"
#include "resolver.h"
#include "interpreter.h"

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


void Resolver::beginScope() {
    unordered_map<std::string, bool> map;
    this->scopes.push(map);
}


void Resolver::endScope() {
    this->scopes.pop();
}

std::any Resolver::visitBlockStmt(Block<std::any>* stmt) {
    beginScope();
    resolve(stmt->statements);
    endScope();
    return nullptr;
}
