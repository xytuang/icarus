#ifndef RESOLVER_H
#define RESOLVER_H

#include <vector>
#include <stack>
#include <unordered_map>

#include "expr.h"
#include "stmt.h"
#include "interpreter.h"

class Resolver : public Expr<std::any>::Visitor<std::any>, public Stmt<std::any>::Visitor<std::any> {
    private:
        Interpreter* interpreter;

        std::stack<std::unordered_map<std::string, bool>> scopes;

        void resolve(Expr<std::any>* expr);

        void resolve(Stmt<std::any>* stmt);

        void resolve(std::vector<Stmt<std::any>*> statements);

        void beginScope();

        void endScope();


    public:
        Resolver(Interpreter* interpreter);

        std::any visitBlockStmt(Block<std::any>* stmt);

};


#endif
