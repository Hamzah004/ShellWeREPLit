/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utls.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 16:20:22 by amufleh           #+#    #+#             */
/*   Updated: 2026/02/18 15:47:43 by amufleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"

int	is_separator(char c)
{
	return (c == ' ' || c == '\t' || c == '|' || c == '<' || c == '>');
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

t_token_type	set_token_type(char *str)
{
	if (!str)
		return (TOK_WORD);
	if (ft_strcmp(str, "|") == 0)
		return (TOK_PIPE);
	if (ft_strcmp(str, "<") == 0)
		return (TOK_REDIR_IN);
	if (ft_strcmp(str, ">") == 0)
		return (TOK_REDIR_OUT);
	if (ft_strcmp(str, ">>") == 0)
		return (TOK_APPEND);
	if (ft_strcmp(str, "<<") == 0)
		return (TOK_HEREDOC);
	return (TOK_WORD);
}

t_tokens	*new_token(char *value, t_token_type type)
{
	t_tokens	*token;

	token = malloc(sizeof(t_tokens));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->prev = NULL;
	token->next = NULL;
	return (token);
}
