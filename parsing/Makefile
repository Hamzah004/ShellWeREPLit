# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/29 09:04:47 by amufleh           #+#    #+#              #
#    Updated: 2026/05/07 11:49:41 by amufleh          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC = command_expansion_utils.c command_expansion_utils2.c command_expansion_utils3.c \
	command_expansion.c debugging_functions.c fill_command.c free_parser.c \
	parser_utils.c parser.c syntax_validation.c tokenizer_utls.c tokenizer.c heredoc.c

CC = cc
CFLAGS = -Wall -Wextra -Werror
OBJ = $(SRC:.c=.o)
NAME = mini
LIBFT = ../libft/libft.a

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -lreadline -o $(NAME)

$(LIBFT):
	make -C libft

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
	make -C libft clean

fclean: clean
	rm -f $(NAME)
	make -C libft fclean

re: fclean all

.PHONY: all clean fclean re
