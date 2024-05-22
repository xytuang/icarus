CXX = g++
CXXFLAGS = -std=c++0x -Wall -g
LDFLAGS = -rdynamic

SRCS = icarus.cpp token.cpp scanner.cpp parser.cpp
OBJS = ${SRCS:.cpp=.o}
HEADERS = 

MAIN = icarus

all: ${MAIN}

${MAIN}: ${OBJS}
	${CXX} ${LDFLAGS} ${OBJS} -o ${MAIN}

.cpp.o:
	${CXX} ${CXXFLAGS} -c $< -o $@

clean:
	${RM} ${PROGS} ${MAIN} ${OBJS} *.o *~. 
