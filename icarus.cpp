#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <sstream>
#include <string>
#include <cstdlib>

#include "icarus.h"
#include "scanner.h"
#include "token.h"
#include "parser.h"
#include "resolver.h"
#include "cleaner.h"
#include "runtime_error.h"
#include "stmt.h"


Interpreter* Icarus::interpreter = new Interpreter(); 

bool Icarus::hadError = false;

bool Icarus::hadRuntimeError = false;

void Icarus::run(std::string source){
    Scanner *scanner = new Scanner(source);
    std::vector<Token *> tokens = scanner->scanTokens();
    Parser<std::any>* parser = new Parser<std::any>(tokens);

    std::vector<Stmt<std::any>*> statements = parser->parse();

    if (hadError) return;

    Resolver* resolver = new Resolver(interpreter);
    resolver->resolve(statements);
    interpreter->interpret(statements);
    
    Cleaner* cleaner = new Cleaner();
    
    cleaner->clean(statements);

    scanner->clean();

    parser->clean();

    delete parser;
    delete resolver;
    delete interpreter;
    delete cleaner;

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
    if (hadError) exit(65);
    if (hadRuntimeError) exit(70);
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
        report(token->getLine(), " at \'" + token->getLexeme() + "\'", message);
    }
}
void Icarus::error(int line, std::string message){
    report(line, "", message);
}

void Icarus::runtimeError(RuntimeError* error) {
    std::cerr << error->what() << std::endl;
    std::cerr << "[line " << error->token->getLine() << + "]" << std::endl;
    hadRuntimeError = true;
}
