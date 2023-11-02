##
## EPITECH PROJECT, 2021
## project
## File description:
## Makefile
##

NAME	=	ecsdemo
NAME_TESTS	=	unit_tests

# src
SRC_DIR	=	src/
SRC	=	$(shell find $(SRC_DIR) -type f '(' -name "*.cpp" ')')
SRC_NO_MAIN	=	$(filter-out $(SRC_DIR)main.c, $(SRC))

# tests
TESTS_DIR	=	tests/
TESTS	=	$(shell find $(TESTS_DIR) -type f '(' -name "*.cpp" ')')

# compil
OBJ	=	$(SRC:.cpp=.o)
CC	=	g++ -g

COUNT	=	0
TOTAL_NBR_FILES	=	$(shell echo $(SRC) | wc -w)

# clean
BIN	=	vgcore*	\
		unit_tests*

# flags
FLAGS	=	-g

CXXFLAGS	+=	-std=c++20 -Wall -Wextra

CSFML_FLAGS	=	-lsfml-graphics	\
				-lsfml-window	\
				-lsfml-system	\
				-lsfml-audio	\
				-lm

TESTS_FLAGS	=	--coverage	\
				-lcriterion

RESET	=	\033[0m
BLACK	=	\033[0;30m
RED		=	\033[0;31m
GREEN	=	\033[0;32m
YELLOW	=	\033[0;33m
BLUE	=	\033[0;34m
PURPLE	=	\033[0;35m
CYAN	=	\033[0;36m
WHITE	=	\033[0;37m
BOLD_BLACK	=	\033[1;30m
BOLD_RED	=	\033[1;31m
BOLD_GREEN	=	\033[1;32m
BOLD_YELLOW	=	\033[1;33m
BOLD_BLUE	=	\033[1;34m
BOLD_PURPLE	=	\033[1;35m
BOLD_CYAN	=	\033[1;36m
BOLD_WHITE	=	\033[1;37m

all:	$(NAME)

$(NAME):	$(OBJ)
	@echo -e "$(BOLD_BLUE)Files compilation ok\n$(RESET)"
	$(CC) -o $(NAME) $(OBJ) $(FLAGS) $(CXXFLAGS) $(CSFML_FLAGS)
	@echo -e "$(BOLD_GREEN)Compilation done$(RESET)"

%.o: %.cpp
	$(eval COUNT=$(shell echo $$(($(COUNT)+1))))
	@echo -e "$(BOLD_CYAN)[$(shell echo $$(( $(COUNT) * 100 / \
	$(TOTAL_NBR_FILES) )))%]$(RESET) Compiling $< -> $@"
	@$(CC) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -f $(BIN)
	rm -f $(OBJ)
	@echo -e "$(BOLD_YELLOW)Cleaning done\n$(RESET)"

fclean:	clean
	rm -f $(NAME)
	@echo -e "$(BOLD_YELLOW)Full cleaning done\n$(RESET)"

re:	clean all

gcovr:
		gcovr --exclude tests
		gcovr --exclude tests --branches

tests_run:	fclean
			gcc -o $(NAME_TESTS) $(SRC_NO_MAIN) $(TESTS) $(TESTS_FLAGS) $(FLAGS)
			./$(NAME_TESTS)

.PHONY:		all clean fclean re gcovr tests_run
