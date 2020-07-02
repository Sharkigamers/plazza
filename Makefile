##
## EPITECH PROJECT, 2020
## Plazza
## File description:
## Makefile
##

PROJECT			=		Plazza
NAME			=		./plazza

SRC				=		$(shell find $(SOURCEDIR) -name '*.cpp' )    \

OBJS			=		$(SRC:.cpp=.o)

################################################################################

CPPFLAGS			+=		-W -Wall -Wextra
CPPFLAGS			+=		-I./include/
CPPFLAGS			+=		-I./src/cooks -I./src/kitchen -I./src/reception -I./src/
CPPFLAGS			+=		-std=c++17
CPPFLAGS			+=		-pthread

################################################################################

SHELL			=		/bin/bash
PRINT			=		printf "$(PROJECT):\t" ; printf
RM				=		rm -f
CC				=		g++

RESET			=		\033[0m
RED				=		\033[0;31m
GREEN			=		\033[0;32m
YELLOW			=		\033[0;33m
BLUE			=		\033[0;34m
MAGENTA			=		\033[0;35m
CYAN			=		\033[1;36m

################################################################################

all: $(NAME)

debug: CPPFLAGS 	+=		-g3 -D DEBUG
debug: $(NAME)

$(NAME): prebuild $(OBJS)
	@ $(PRINT) "$(YELLOW)%-40b$(RESET)" "Building $(NAME)"
	@ $(CC) -o $(NAME) $(OBJS) $(CPPFLAGS) && \
	  (printf "$(GREEN)Done$(RESET)\n" ; exit 0) || \
	  (printf "$(RED)Fail$(RESET)\n" ; exit 1)

prebuild:
	@ $(PRINT) "$(YELLOW)%b$(RESET)\n" "Compiling source files"

clean:
	@ $(PRINT) "$(YELLOW)%-40b$(RESET)" "Deleting object files"
	@ $(RM) $(OBJS)
	@ printf "$(GREEN)Done$(RESET)\n"

fclean: clean
	@ $(PRINT) "$(YELLOW)%-40b$(RESET)" "Deleting $(NAME)"
	@ $(RM) $(NAME)
	@ printf "$(GREEN)Done$(RESET)\n"

re: fclean all
de: fclean debug

.c.o:
	@ $(CC) -c $< -o $@ $(CPPFLAGS) && \
	  ($(PRINT) "  $(GREEN)[OK]$(CYAN)  %b$(RESET)\n" $<) || \
	  ($(PRINT) "  $(RED)[XX]$(CYAN)  %b$(RESET)\n" $< ; exit 1)

.PHONY: all $(NAME) prebuild clean fclean re .c.o