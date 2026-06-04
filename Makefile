# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/11 16:28:02 by hbani-at          #+#    #+#              #
#    Updated: 2026/06/04 23:40:57 by hbani-at         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ================================== CONFIG ================================== #

NAME = ShellWeREPLit

CC = cc
CFLAGS = -Wall -Werror -Wextra -I./include
RM = rm -f

# =============================== OUTPUT COLORS ============================== #

COM_COLOR	= \033[0;34m
OK_COLOR	= \033[0;32m
NO_COLOR	= \033[m
COM_STRING	= Compiling
OK_STRING	= [OK]

# ================================ DIRECTORIES =============================== #

EXECUTION_DIR		= ./execution
PARSING_DIR		= ./parsing
BUILTIN_DIR		= ./builtin_funcitons
SIGNAL_HANDLE_DIR	= ./signal_handling
LIBFT_DIR		= ./libft
LIBFT_A			= $(LIBFT_DIR)/libft.a

# ================================== SOURCES ================================= #

SRC =	main.c \
	error.c \
	custom_prompt.c \
	$(EXECUTION_DIR)/execution.c \
	$(EXECUTION_DIR)/env_init.c \
	$(EXECUTION_DIR)/command_path.c \
	$(EXECUTION_DIR)/env_set.c \
	$(EXECUTION_DIR)/env_array.c \
	$(EXECUTION_DIR)/execution_utils.c \
	$(EXECUTION_DIR)/builtins_dispatch.c \
  	$(EXECUTION_DIR)/exec_single.c \
  	$(EXECUTION_DIR)/exec_pipeline.c \
  	$(EXECUTION_DIR)/exec_child.c \
	$(EXECUTION_DIR)/handle_redir.c \
	$(SIGNAL_HANDLE_DIR)/signal.c \
	$(BUILTIN_DIR)/cd_builtin.c \
	$(BUILTIN_DIR)/echo_builtin.c \
	$(BUILTIN_DIR)/env_builtin.c \
	$(BUILTIN_DIR)/exit_builtin.c \
	$(BUILTIN_DIR)/export_builtin.c \
	$(BUILTIN_DIR)/pwd_builtin.c \
	$(BUILTIN_DIR)/unset_builtin.c \
	$(BUILTIN_DIR)/helpers.c \
	$(PARSING_DIR)/tokenizer.c \
	$(PARSING_DIR)/tokenizer_utls.c \
	$(PARSING_DIR)/command_expansion.c \
	$(PARSING_DIR)/command_expansion_utils.c \
	$(PARSING_DIR)/free_parser.c \
	$(PARSING_DIR)/parser.c \
	$(PARSING_DIR)/parser_utils.c \
	$(PARSING_DIR)/syntax_validation.c \
	$(PARSING_DIR)/command_expansion_utils2.c \
	$(PARSING_DIR)/command_expansion_utils3.c \
	$(PARSING_DIR)/fill_command.c \
	$(PARSING_DIR)/heredoc.c \
	$(PARSING_DIR)/heredoc_utils.c

OBJS = $(SRC:.c=.o)

# =================================== RULES ================================== #
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) -lreadline -o $(NAME)
	@printf "%b" "$(OK_COLOR)$(OK_STRING) final ShellWeREPLit executable successfully created\n$(NO_COLOR)"

$(LIBFT_A):
	@make --no-print-directory -C $(LIBFT_DIR) all bonus
	@printf "%b" "$(OK_COLOR)$(OK_STRING) libft archive created\n$(NO_COLOR)"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf "%b" "$(COM_COLOR)$(COM_STRING) $<\n$(NO_COLOR)"

clean:
	@$(RM) $(OBJS)
	@printf "%b" "$(OK_COLOR)$(OK_STRING) minishell object files cleaned\n$(NO_COLOR)"
	@make --no-print-directory -C $(LIBFT_DIR) clean

fclean: clean
	@$(RM) $(NAME)
	@printf "%b" "$(OK_COLOR)$(OK_STRING) minishell executable cleaned\n$(NO_COLOR)"
	@make --no-print-directory -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
