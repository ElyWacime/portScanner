CXX = c++
CXXFLAGS = -Wall -Wextra -Werror 
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
	rm -f ${PROGRAM} output.txt

re: fclean all

.PHONY: re clean fclean all
