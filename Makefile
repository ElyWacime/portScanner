CXX = c++
CXXFLAGS = -Wall -Wextra -Werror
CXXFILES = portScanner.cpp main.cpp
CXXOBJ = $(CXXFILES:.cpp=.o)
PROGRAM = portScanner

all: ${PROGRAM}

${PROGRAM}: ${CXXOBJ}
	${CXX} ${CXXFLAGS} ${CXXOBJ} -o ${PROGRAM}

%.o:%.cpp portScanner.hpp
	${CXX} ${CXXFLAGS} -c $< -o $@

clean:
	rm ${CXXOBJ}

fclean: clean
	rm ${PROGRAM}

re: fclean all

.PHONY: re clean fclean all
