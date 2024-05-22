#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>

#include "icarus.h"

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

int main(int argc, char** argv){
    signal(SIGSEGV, handler);
    if (argc > 2) {
        std::cout << "Usage: icarus [script]" << std::endl;
        exit(1);
    }
    else if (argc == 2) {
        Icarus::runFile(argv[1]);
    }
    else {
        Icarus::runPrompt();
    }
}
