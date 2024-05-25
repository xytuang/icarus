#include <vector>
#include <string>
#include <iostream>
#include "expr.h"
#include "token.h"
#include "tokentype.h"

class AstPrinter : public Expr<std::string>::Visitor<std::string> {
    private:
        std::string parenthesize(std::string name, std::initializer_list<Expr<std::string>*> exprs) {
            std::string result = "(";
            result += name;
            std::vector<Expr<std::string>*> vec(exprs);
            for (int i = 0; i < vec.size(); i++) {
                result += " ";
                result += vec[i]->accept(this);
            }
            result += ")";
            return result;
        }
    public:
        std::string print(Expr<std::string>* expr) {
            return expr->accept(this);
        }
        std::string visitBinaryExpr(Binary<std::string>* expr) {
            return parenthesize(expr->operation->getLexeme(), {expr->left, expr->right});
        }

        std::string visitGroupingExpr(Grouping<std::string>* expr) {
            return parenthesize("group", {expr->expression});
        }

        std::string visitLiteralExpr(Literal<std::string>* expr) {
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

        std::string visitUnaryExpr(Unary<std::string>* expr) {
            return parenthesize(expr->operation->getLexeme(), {expr->right});
        }
};

int main() {
    Expr<std::string>* expression = new Binary<std::string>(
        new Unary<std::string>(
            new Token(MINUS, "-", nullptr, 1),
            new Literal<std::string>(123)),
        new Token(STAR, "*", nullptr, 1),
        new Grouping<std::string>(
            new Literal<std::string>(45.67)));
    AstPrinter* printer = new AstPrinter();
    std::string result = printer->print(expression);
    std::cout << result  << std::endl;
  
}
