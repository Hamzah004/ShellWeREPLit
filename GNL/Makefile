# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/20 01:29:42 by hbani-at          #+#    #+#              #
#    Updated: 2025/09/20 01:34:59 by hbani-at         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

COM_COLOR   = \033[0;34m
OK_COLOR    = \033[0;32m
NO_COLOR    = \033[m

OK_STRING    = "[OK]"
COM_STRING   = "Compiling"

NAME = gnl.a
CC = cc
CFLAGS = -Wall -Werror -Wextra
SRC =	get_next_line.c \
	get_next_line_utils.c \
	get_next_line_bonus.c \
	get_next_line_utils_bonus.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@ar rcs $(NAME) $(OBJ)
	@printf "$(OK_COLOR)$(OK_STRING) GNL archive cerated\n$(NO_COLOR)"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@ 
	@printf "%b" "$(COM_COLOR)$(COM_STRING) $<\n$(NO_COLOR)"

clean:
	@rm -f $(OBJ)
	@printf "%b" "$(OK_COLOR)$(OK_STRING) GNL Object files cleaned\n$(NO_COLOR)"

fclean: clean
	@rm -f $(NAME)
	@printf "%b" "$(OK_COLOR)$(OK_STRING) GNL archive cleaned\n$(NO_COLOR)"

re: fclean all

.PHONY: all clean fclean re
