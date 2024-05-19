CXX = g++
CXXFLAGS = -std=c++0x -Wall -pedantic-errors -g

SRCS = icarus.cpp token.cpp scanner.cpp
OBJS = ${SRCS:.cpp=.o}
HEADERS = 

MAIN = icarus

all: ${MAIN}

${MAIN}: ${OBJS}
	${CXX} ${CXXFLAGS} ${OBJS} -o ${MAIN}

.cpp.o:
	${CXX} ${CXXFLAGS} -c $< -o $@

clean:
	${RM} ${PROGS} ${MAIN} ${OBJS} *.o *~. 
