#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <sstream>

#include <stdlib.h>

bool hadError = false;

std::vector<std::string> splitString(const std::string str, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;

    while(std::getline(iss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void run(std::string source){
    std::vector<std::string> tokens = splitString(source, ' ');
    for (std::string str : tokens) {
        std::cout << str << std::endl;
    }
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
