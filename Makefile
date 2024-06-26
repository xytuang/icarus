CXX = g++
CXXFLAGS = -std=c++17 -Wall -g
LDFLAGS = -rdynamic

SRCS = main.cpp icarus.cpp token.cpp scanner.cpp interpreter.cpp resolver.cpp cleaner.cpp
OBJS = ${SRCS:.cpp=.o}
HEADERS = 

MAIN = main

all: ${MAIN}

${MAIN}: ${OBJS}
	${CXX} ${LDFLAGS} ${OBJS} -o ${MAIN}

.cpp.o:
	${CXX} ${CXXFLAGS} -c $< -o $@

clean:
	${RM} ${PROGS} ${MAIN} ${OBJS} *.o *~. 
