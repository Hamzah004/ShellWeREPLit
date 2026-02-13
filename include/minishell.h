/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 16:18:49 by hbani-at          #+#    #+#             */
/*   Updated: 2026/02/11 16:24:00 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

typedef enum e_error
{
	ERROR_SUCCESS = 0,
	ERROR_INVALID_ARGS = 1,
	ERROR_INVALID_PID = 2,
	ERROR_INVALID_KILL = 3
}					t_error;

typedef enum e_token_type
{
	TOK_WORD,
	TOK_PIPE,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_APPEND,
	TOK_HEREDOC,
	TOK_SINGLE_QUOTE,
	TOK_DOUBLE_QUOTE
}					t_token_type;

typedef struct s_cmd
{
	char			*cmd;
}					t_cmd;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}					t_tokens;

void				print_error(t_error error);

#endif
