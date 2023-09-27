##
## EPITECH PROJECT, 2021
## project
## File description:
## Makefile
##

NAME    =    r-type_client

# src
SRC = $(shell find . -type f -name "*.cpp")

# compilation
OBJ    =    $(SRC:.cpp=.o)
CC    =    g++

COUNT    =    0
TOTAL_NBR_FILES    =    $(shell echo $(SRC) | wc -w)

# clean
BIN    =    vgcore*    \
        unit_tests*

# flags
FLAGS    =    -g

UNAME_S := $(shell uname -s)

# Flags Linux
CFLAGS += -std=c++2a -Wall -Wextra -lsfml-graphics -lsfml-window -lsfml-system -lpthread

# Flags macOS
ifeq ($(UNAME_S),Darwin)
    CFLAGS += -I/opt/homebrew/Cellar/sfml/2.6.0/include -L/opt/homebrew/Cellar/sfml/2.6.0/lib -I/opt/homebrew/Cellar/asio/1.28.1/include
endif

RESET    =    \033[0m
BLACK    =    \033[0;30m
RED        =    \033[0;31m
GREEN    =    \033[0;32m
YELLOW    =    \033[0;33m
BLUE    =    \033[0;34m
PURPLE    =    \033[0;35m
CYAN    =    \033[0;36m
WHITE    =    \033[0;37m
BOLD_BLACK    =    \033[1;30m
BOLD_RED    =    \033[1;31m
BOLD_GREEN    =    \033[1;32m
BOLD_YELLOW    =    \033[1;33m
BOLD_BLUE    =    \033[1;34m
BOLD_PURPLE    =    \033[1;35m
BOLD_CYAN    =    \033[1;36m
BOLD_WHITE    =    \033[1;37m

all:    $(NAME)

$(NAME):    $(OBJ)
	@echo -e "$(BOLD_BLUE)Files compilation ok\n$(RESET)"
	$(CC) -o $(NAME) $(OBJ) $(FLAGS) $(CFLAGS)
	@echo -e "$(BOLD_GREEN)Compilation done$(RESET)"

%.o: %.cpp
	$(eval COUNT=$(shell echo $$(($(COUNT)+1))))
	@echo -e "$(BOLD_CYAN)[$(shell echo $$(( $(COUNT) * 100 / \
	$(TOTAL_NBR_FILES) )))%]$(RESET) Compiling $< -> $@"
	@$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f $(BIN)
	rm -f $(OBJ)
	@echo -e "$(BOLD_YELLOW)Cleaning done\n$(RESET)"

fclean:    clean
	rm -f $(NAME)
	@echo -e "$(BOLD_YELLOW)Full cleaning done\n$(RESET)"

re:    clean all

.PHONY:        all clean fclean re
