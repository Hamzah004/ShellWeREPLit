/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 16:25:30 by amufleh           #+#    #+#             */
/*   Updated: 2026/02/14 13:44:05 by amufleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include "./libft/libft.h"

typedef enum token_type
{
	TOK_WORD,
	TOK_PIPE,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_APPEND,
	TOK_HEREDOC,
}	t_token_type;

// typedef struct s_redic
// {
// 	char *name;
// 	enum type;
// } t_redic;

// typedef struct s_command_info
// {
// 	char *command;
// 	char **args;
// 	t_redic input;
// 	t_redic output;

// } t_command_info;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
	struct s_token	*prev;
}	t_tokens;

int				free_tokens(t_tokens *head);
int				is_separator(char c);
int				is_operator(char c);
int				add_token(t_tokens **tokens, char *value);
int				extract_tokin(t_tokens **my_tokens, char *command,
					int start, int end);
int				handel_operators(t_tokens **my_tokens, char *command, int *i);
int				handle_word(t_tokens **my_tokens, char *command, int *i);
int				token_analyser(char *command, t_tokens **my_tokens);
t_token_type	set_token_type(char *str);
t_tokens		*new_token(char *value, t_token_type type);

#endif
