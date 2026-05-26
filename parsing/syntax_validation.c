/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 12:06:23 by amufleh           #+#    #+#             */
/*   Updated: 2026/05/09 16:44:59 by amufleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"

int	is_redirection(t_token_type type)
{
	if (type == TOK_REDIR_IN || type == TOK_REDIR_OUT
		|| type == TOK_APPEND || type == TOK_HEREDOC)
		return (1);
	return (0);
}

static int	syntax_error(t_tokens *tok)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (tok)
		ft_putstr_fd(tok->value, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putendl_fd("'", 2);
	return (0);
}

int	syntax_validation(t_tokens *my_tokens)
{
	t_tokens	*token;

	token = my_tokens;
	while (token)
	{
		if (token->type == TOK_PIPE)
		{
			if (!token->prev)
				return (syntax_error(token));
			if (!token->next)
				return (syntax_error(NULL));
			if (token->prev->type != TOK_WORD || token->next->type == TOK_PIPE)
				return (syntax_error(token->next));
		}
		if (is_redirection(token->type))
		{
			if (!token->next)
				return (syntax_error(NULL));
			if (is_redirection(token->next->type)
				|| token->next->type == TOK_PIPE)
				return (syntax_error(token->next));
		}
		token = token->next;
	}
	return (1);
}
