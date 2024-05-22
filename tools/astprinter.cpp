#include <vector>
#include <string>
#include <iostream>
#include "expr.h"
#include "token.h"
#include "tokentype.h"

class AstPrinter : public Expr::Visitor {
    private:
        // Recursive function to handle variadic arguments and append them to a vector
        void appendExprs(std::vector<Expr*>& vec) {
            // Base case: No arguments to append, do nothing
        }

        template<typename T, typename... Args>
        void appendExprs(std::vector<Expr*>& vec, T first, Args... args) {
            vec.push_back(first); // Append the first argument to the vector
            appendExprs(vec, args...); // Recursively handle the rest of the arguments
        }
        std::string parenthesize(std::string name, std::initializer_list<Expr*> exprs) {
            std::string result = "(";
            result += name;
            std::vector<Expr*> vec(exprs);
            for (int i = 0; i < vec.size(); i++) {
                result += " ";
                result += vec[i]->accept(this);
            }
            result += ")";
            return result;
        }
    public:
        std::string print(Expr* expr) {
            return expr->accept(this);
        }
        std::string visitBinaryExpr(Binary* expr) {
            return parenthesize(expr->operation->getLexeme(), {expr->left, expr->right});
        }

        std::string visitGroupingExpr(Grouping* expr) {
            return parenthesize("group", {expr->expression});
        }

        std::string visitLiteralExpr(Literal* expr) {
            if (expr->value == "") {
                return "nil";
            }
            return expr->value;
        }

        std::string visitUnaryExpr(Unary* expr) {
            return parenthesize(expr->operation->getLexeme(), {expr->right});
        }
};


int main() {
    Expr* expression = new Binary(
        new Unary(
            new Token(MINUS, "-", "", 1),
            new Literal("123")),
        new Token(STAR, "*", "", 1),
        new Grouping(
            new Literal("45.67")));
    AstPrinter * printer = new AstPrinter();
    std::string result = printer->print(expression);
    std::cout << result  << std::endl;
  
}
