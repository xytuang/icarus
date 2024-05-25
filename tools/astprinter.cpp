#include <vector>
#include <string>
#include <iostream>
#include "expr.h"
#include "token.h"
#include "tokentype.h"

class AstPrinter : public Expr::Visitor {
    private:
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
            std::string valueAsString;
            if (expr->value.has_value()) {
                if (expr->value.type() == typeid(std::string)) {
                    valueAsString = std::any_cast<std::string>(expr->value);
                    return valueAsString;
                }
                else if (expr->value.type() == typeid(int)) {
                    valueAsString = std::to_string(std::any_cast<int>(expr->value));
                    return valueAsString;
                }

                else if (expr->value.type() == typeid(double)) {
                    valueAsString = std::to_string(std::any_cast<double>(expr->value));
                    return valueAsString;
                }
                else {
                    return "Unknown type";
                }
            }
            return "nil";
            /*
            try {
                return std::any_cast<std::string>(expr->value); 
            } catch(const std::bad_any_cast&) {
                return "nil";
            }
            */

        }

        std::string visitUnaryExpr(Unary* expr) {
            return parenthesize(expr->operation->getLexeme(), {expr->right});
        }
};


int main() {
    Expr* expression = new Binary(
        new Unary(
            new Token(MINUS, "-", nullptr, 1),
            new Literal(123)),
        new Token(STAR, "*", nullptr, 1),
        new Grouping(
            new Literal(45.67)));
    AstPrinter * printer = new AstPrinter();
    std::string result = printer->print(expression);
    std::cout << result  << std::endl;
  
}
