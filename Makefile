CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g
CXXFILES = portScanner.cpp main.cpp utils.cpp
CXXOBJ = $(CXXFILES:.cpp=.o)
PROGRAM = portScanner

all: ${PROGRAM}

${PROGRAM}: ${CXXOBJ}
	${CXX} ${CXXFLAGS} ${CXXOBJ} -o ${PROGRAM}

%.o:%.cpp portScanner.hpp
	${CXX} ${CXXFLAGS} -c $< -o $@

clean:
	rm  -f ${CXXOBJ}

fclean: clean
	rm -f ${PROGRAM}

re: fclean all

.PHONY: re clean fclean all
