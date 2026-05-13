/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 16:25:30 by amufleh           #+#    #+#             */
/*   Updated: 2026/05/09 17:31:30 by amufleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "../libft/libft.h"
# include <readline/readline.h>

typedef enum e_token_type
{
	TOK_WORD,
	TOK_PIPE,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_APPEND,
	TOK_HEREDOC
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
	int	heredoc_fd;
	t_token_type	type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**argv;
	t_redir			*redirection;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}	t_commands;

typedef struct s_envp
{
	char			*key;
	char			*value;
	struct s_envp	*next;
}	t_envp;

typedef struct s_track_quote
{
	int	i;
	int	s_quote;
	int	d_quote;
	int	was_quoted;
}	t_track_quote;

// TOKENIZER

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
int				syntax_validation(t_tokens *my_tokens);
int				is_redirection(t_token_type type);

// DEBUG

void			print_tokens(t_tokens *head);
void			print_redirections(t_redir *redir);
void			print_commands(t_commands *cmd);

// PARSER

t_redir			*new_redirection(t_token_type type, char *value);
t_commands		*new_command(void);
int				fill_command_options(t_commands *my_commands,
					int *size, char *token_value);
int				add_redir(t_redir **redirection, t_tokens *token);
int				add_command(t_commands **command, int *size);
int				the_parser(t_tokens *my_tokens, t_commands *my_commands);
int				parse_token(t_commands **my_commands,
					t_tokens **token, int *size);
void			free_commands(t_commands *my_commands);
t_commands		*free_parser(t_commands *my_commands, t_tokens *my_tokens);
void			free_redirections(t_redir *redir_list);
void			*ft_realloc(void *ptr, size_t old_size, size_t new_size);
t_commands		*fill_command_struct(char *input, t_envp *env,
					int exit_status);
int				collect_heredocs(t_commands *cmds, t_envp *env,
					int exit_status);
// EXPANSION

char			**expand_argv(char *input, t_envp *env, int exit_status);
int				handle_expansion(t_commands *cmd, t_envp *env,
					int exit_status, int *i);
char			**handel_double_quote(t_track_quote *tracker, char *input,
					char **result, t_envp *env, int exit_status);
char			**handel_single_quote(char *input, int *i, char **result);
char			*handle_var(char *input, t_track_quote *tracker, t_envp *env,
					int exit_status);
char			*ft_strndup(const char *s, size_t n);
char			*get_env_value(char *name, t_envp *env);
char			*append(char *str, char c);
int				yokotenkai(t_commands *my_commands, t_envp *env,
					int exit_status);
char			**quote_tracker(t_track_quote *tracker, char *input,
					char **result, t_envp *env, int exit_status);
char			*expand_files(char *file);
char			*replace_str(char *old);
char			*polish(char *value);
int				remove_empty_arg(t_commands *my_commands);
int				only_space(char *str);
int				count_words(char **arr);
char			**insert_argv(char **argv, char **words);
char			**insert_words(char **argv, int index, char **words);
char			*strjoin_free(char	*old, char	*new);
char			**handle_unquoted_var(char *tmp, char **result);
char			*handle_env_value(char *name, t_envp *env);

//FREE

t_commands		*free_parser(t_commands *my_commands, t_tokens *my_tokens);
int				free_tokens(t_tokens *head);
void			free_commands(t_commands *my_commands);
void			free_redirections(t_redir *redir_list);
char			**free_argv(char **argv);

#endif
