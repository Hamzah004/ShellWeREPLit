/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 16:25:30 by amufleh           #+#    #+#             */
/*   Updated: 2026/02/18 16:28:20 by amufleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include "../libft/libft.h"

typedef enum token_type
{
	TOK_WORD,
	TOK_PIPE,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_APPEND,
	TOK_HEREDOC,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
	struct s_token	*prev;
}	t_tokens;

typedef struct s_redir
{
	t_token_type type;
	char *file;
	struct s_redir *next;
} t_redir;

typedef struct s_cmd
{
	char **argv;
	t_redir *redirection;
	struct s_cmd *next;
	struct s_cmd *prev;
} t_commands;

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

//------------- ---------------------------------------------------

int syntax_validation (t_tokens *my_tokens);
int	is_redirection (t_token_type type);

//------------------------------------------------------------------

void	print_tokens(t_tokens *head);
void print_redirections(t_redir *redir);
void print_commands(t_commands *cmd);

//------------------------------------------------------------------

t_redir	*new_redirection(t_token_type type, char *value);
t_commands	*new_command(t_tokens *tokens);
int	fill_command_options(t_commands *my_commands,
	int *size, char *tokens_value);
int	add_redir(t_redir **redirection, t_tokens *token);
int	add_command(t_commands **command, int *size, t_tokens *tokens);
int the_parser(t_tokens *my_tokens, t_commands *my_commands);
int	count_words(t_tokens *token);

#endif
