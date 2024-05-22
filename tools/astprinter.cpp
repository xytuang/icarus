#include <vector>
#include <string>
#include <iostream>
#include "expr.h"
#include "tokentype.h"

class AstPrinter : public Expr::Visitor {
    private:

        template<class... Expr>
        std::string parenthesize(std::string name, Expr... exprs) {
            std::string result = "(";
            result += name;
            for (Expr expr : exprs) {
                result += " ";
                result += expr.accept(this);
            }
            result += ")";
            return result;
        }
    public:
        std::string print(Expr expr) {
            return expr.accept(this);
        }
        std::string visitBinaryExpr(Binary* expr) {
            return parenthesize(expr->operation->getLexeme(), expr->left, expr->right);
        }

        std::string visitGroupingExpr(Grouping* expr) {
            return parenthesize("group", expr->expression);
        }


        std::string visitLiteralExpr(Literal* expr) {
            if (expr->value == "") {
                return "nil";
            }
            return expr->value;
        }

        std::string visitUnaryExpr(Unary* expr) {
            return parenthesize(expr->operation->getLexeme(), expr->right);
        }
};


int main() {
    Expr expression = new Binary(
        new Unary(
            new Token(TokenType.MINUS, "-", "", 1),
            new Literal(123)),
        new Token(TokenType.STAR, "*", "", 1),
        new Grouping(
            new Literal(45.67)));
    AstPrinter * printer = new AstPrinter();
    std::string result = printer->print(expression);
    std::cout << result  << std::endl;
  
}
