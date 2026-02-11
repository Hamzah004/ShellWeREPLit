/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utls.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 16:20:22 by amufleh           #+#    #+#             */
/*   Updated: 2026/02/11 16:27:09 by amufleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"


int	ft_strcmp(const char *s1, const char *s2)
{
	int i;

	i = 0;
	if (!s1 || !s2)
		return (1);
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

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
	token->next = NULL;
	return (token);
}
