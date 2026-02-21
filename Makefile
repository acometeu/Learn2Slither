SRC = main.cpp Board.cpp Snake.cpp utils.cpp

OBJDIR = OBJ
OBJ	= ${addprefix ${OBJDIR}/,  ${SRC:.cpp=.o}}

DEPENDS = ${SRC:.cpp=.d}

NAME = Learn2Slither

CXX = c++

CXXLAGS = -std=c++20 -Wall -Werror -Wextra -g

RM = rm -rf

all: ${NAME}

${NAME}: ${OBJ}
	${CXX} ${CXXLAGS} ${OBJ} -o ${NAME}

clean:
	${RM} ${OBJDIR}

fclean:	clean
	${RM} ${NAME}

re: fclean ${NAME}

${OBJDIR}:
	mkdir -p ${OBJDIR}

-include $(DEPENDS)

${OBJDIR}/%.o : %.cpp Makefile | ${OBJDIR}
	${CXX} ${CXXLAGS} -MD -MP -c $< -o $@ 

.PHONY:	all clean fclean re		#.PHONY means these rules get executed even if files of those names exist.