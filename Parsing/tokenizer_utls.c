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

int	free_tokens(t_tokens *head)
{
	t_tokens	*tmp;

	while (head)
	{
		tmp = head->next;
		free(head->value);
		free(head);
		head = tmp;
	}
	return (0);
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

char	*remove_quotes(char *str)
{
	char	*result;
	int		len;
	int		i;
	int		j;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if ((str[0] == '\'' || str[0] == '\"') && str[0] == str[len - 1] && len >= 2)
	{
		result = malloc(len - 1);
		if (!result)
			return (NULL);
		i = 1;
		j = 0;
		while (i < len - 1)
		{
			result[j] = str[i];
			i++;
			j++;
		}
		result[j] = '\0';
		free(str);
		return (result);
	}
	return (str);
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
