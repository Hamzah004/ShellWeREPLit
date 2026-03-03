/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 12:06:23 by amufleh           #+#    #+#             */
/*   Updated: 2026/03/03 11:49:18 by amufleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	is_redirection(t_token_type type)
{
	if (type == TOK_REDIR_IN || type == TOK_REDIR_OUT
		|| type == TOK_APPEND || type == TOK_HEREDOC)
		return (1);
	return (0);
}

int	syntax_validation(t_tokens *my_tokens)
{
	t_tokens	*token;

	token = my_tokens;
	while (token)
	{
		if (token -> type == TOK_PIPE)
		{
			if (!token->prev || !token->next)
				return (0);
			if (token->prev->type != TOK_WORD
				|| token->next->type == TOK_PIPE)
				return (0);
		}
		if (is_redirection(token->type))
		{
			if (!token->next || is_redirection(token->next->type)
				|| token->next->type == TOK_PIPE)
				return (0);
		}
		token = token -> next;
	}
	return (1);
}
