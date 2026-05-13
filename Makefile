# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/11 16:28:02 by hbani-at          #+#    #+#              #
#    Updated: 2026/05/09 17:40:13 by amufleh          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

COM_COLOR   = \033[0;34m
OK_COLOR    = \033[0;32m
NO_COLOR    = \033[m

OK_STRING    = "[OK]"
COM_STRING   = "Compiling"

CC = cc
CFLAGS = -g3 -Wall -Werror -Wextra -I./include
NAME = minishell

EXECUTION_DIR = ./execution
PARSING_DIR = ./parsing
LIBFT_DIR = ./libft
LIBFT_A = $(LIBFT_DIR)/libft.a

SRC =	main.c \
	error.c \
	$(EXECUTION_DIR)/execution.c \
	$(EXECUTION_DIR)/env.c \
	$(EXECUTION_DIR)/env_helper.c \
	$(EXECUTION_DIR)/builtin_functions.c \
	$(EXECUTION_DIR)/execution_utils.c \
	$(EXECUTION_DIR)/handle_redir.c \
	$(EXECUTION_DIR)/env1.c \
	$(SIGNAL_HANDLE_DIR)/signal.c \
	$(BUILTIN_DIR)/cd_builtin.c \
	$(BUILTIN_DIR)/echo_builtin.c \
	$(BUILTIN_DIR)/env_builtin.c \
	$(BUILTIN_DIR)/exit_builtin.c \
	$(BUILTIN_DIR)/export_builtin.c \
	$(BUILTIN_DIR)/pwd_builtin.c \
	$(BUILTIN_DIR)/unset_builtin.c \
	$(PARSING_DIR)/tokenizer.c \
	$(PARSING_DIR)/tokenizer_utls.c \
	$(PARSING_DIR)/command_expansion.c \
	$(PARSING_DIR)/command_expansion_utils.c \
	$(PARSING_DIR)/debugging_functions.c \
	$(PARSING_DIR)/free_parser.c \
	$(PARSING_DIR)/parser.c \
	$(PARSING_DIR)/parser_utils.c \
	$(PARSING_DIR)/syntax_validation.c \
	$(PARSING_DIR)/command_expansion_utils2.c \
	$(PARSING_DIR)/command_expansion_utils3.c \
	$(PARSING_DIR)/fill_command.c \
	$(PARSING_DIR)/heredoc.c

OBJS = $(SRC:.c=.o)
BUILTIN_DIR = ./builtin_funcitons/
SIGNAL_HANDLE_DIR = ./signal_handling/

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) -lreadline -o $(NAME)
	@printf "$(OK_COLOR)$(OK_STRING) final minishell executable successfully created\n$(NO_COLOR)"

$(LIBFT_A):
	@make --no-print-directory -C $(LIBFT_DIR) all bonus
	@printf "$(OK_COLOR)$(OK_STRING) libft archive cerated\n$(NO_COLOR)"

%.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf "%b" "$(COM_COLOR)$(COM_STRING) $<\n$(NO_COLOR)"

clean:
	@rm -f $(OBJS)
	@printf "%b" "$(OK_COLOR)$(OK_STRING) minishell Object files cleaned\n$(NO_COLOR)";
	@make --no-print-directory -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@printf "%b" "$(OK_COLOR)$(OK_STRING) minishell executable cleaned\n$(NO_COLOR)"
	@make --no-print-directory -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
