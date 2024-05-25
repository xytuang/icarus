#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <sstream>
#include <string>

#include "icarus.h"
#include "scanner.h"
#include "token.h"
#include "parser.h"
#include "astprinter.h"

bool Icarus::hadError = false;

void Icarus::run(std::string source){
    Scanner *scanner = new Scanner(source);
    std::vector<Token *> tokens = scanner->scanTokens();
    Parser<std::string>* parser = new Parser<std::string>(tokens);
    Expr<std::string>* expression = parser->parse();

    if (hadError) return;

    std::cout << (new AstPrinter())->print(expression) << std::endl;

}

void Icarus::runFile(char *filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        exit(1);
    }

    std::vector<char> fileContents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    std::string fileString(fileContents.begin(), fileContents.end());
    run(fileString);
    if (hadError) exit(1);
}


void Icarus::runPrompt(){
    std::string line;
    while(true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) {
            break;
        }
        run(line);
        hadError = false;
    }
}

void Icarus::report(int line, std::string where, std::string message) {
    std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
    hadError = true;
}

void Icarus::error(Token* token, std::string message){
    if (token->getType() == END_OF_FILE){
        report(token->getLine(), " at end ",  message);
    }
    else {
        report(token->getLine(), " at \'" + std::to_string(token->getLine()) + "\'", message);
    }
}
void Icarus::error(int line, std::string message){
    report(line, "", message);
}
