SRC = main.cpp Board.cpp Snake.cpp utils.cpp

OBJ	= ${SRC:.cpp=.o}

NAME = Learn2Slither

CXX = c++

CFLAGS = -std=c++20 -Wall -Werror -Wextra -g

RM = rm -rf

%.o : %.cpp
	${CXX} ${CFLAGS} -c $< -o $@ 

all: ${NAME}

${NAME}: ${OBJ}
	${CXX} ${CFLAGS} ${OBJ} -o ${NAME}

clean:
	${RM} ${OBJ}

fclean:	clean
	${RM} ${NAME}

re: fclean ${NAME}

.PHONY:	all clean fclean re