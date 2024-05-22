#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <sstream>

#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include "scanner.h"
#include "token.h"
#include "parser.h"

bool hadError = false;

void handler(int sig) {
    void *array[10];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}
void run(std::string source){
    Scanner *scanner = new Scanner(source);
    std::vector<Token *> tokens = scanner->scanTokens();
    Parser* parser = new Parser(tokens);
    Expr* expression = parser->parse();

    if (hadError) return;

    //std::cout << new AstPrinter()->print(expression) << std::endl;

}


void runFile(char *filename) {
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


void runPrompt(){
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

int main(int argc, char** argv){
    signal(SIGSEGV, handler);
    if (argc > 2) {
        std::cout << "Usage: icarus [script]" << std::endl;
        exit(1);
    }
    else if (argc == 2) {
        runFile(argv[1]);
    }
    else {
        runPrompt();
    }
}
