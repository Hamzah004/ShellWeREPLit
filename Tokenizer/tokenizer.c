/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 16:20:29 by amufleh           #+#    #+#             */
/*   Updated: 2026/02/11 16:20:29 by amufleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

int	add_token(t_tokens **tokens, char *value)
{
	t_tokens		*new;
	t_tokens		*tmp;
	t_token_type	type;

	type = set_token_type(value);
	new = new_token(value, type);
	if (!new)
		return (0);
	if (!*tokens)
		*tokens = new;
	else
	{
		tmp = *tokens;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (1);
}

int	extract_tokin(t_tokens **my_tokens, char *command, int start, int end)
{
	char	*token;

	token = ft_substr(command, start, end);
	if (!token)
		return (0);
	if (!add_token(my_tokens, token))
		return (0);
	return (1);
}

int	handel_operators(t_tokens **my_tokens, char *command, int *i)
{
	if ((command[*i] == '<' && command[*i + 1] == '<')
		|| (command[*i] == '>' && command[*i + 1] == '>'))
	{
		if (!extract_tokin(my_tokens, command, *i, 2))
			return (0);
		*i += 2;
	}
	else
	{
		if (!extract_tokin(my_tokens, command, *i, 1))
			return (0);
		*i += 1;
	}
	return (1);
}

int	handle_word(t_tokens **my_tokens, char *command, int *i)
{
	int		start;
	char	quote;

	start = *i;
	while (command[*i] && !is_separator(command[*i]))
	{
		if (command[*i] == '\'' || command[*i] == '\"')
		{
			quote = command[(*i)++];
			while (command[*i] && command[*i] != quote)
				(*i)++;
		}
		if (command[*i])
			(*i)++;
	}
	if (!extract_tokin(my_tokens, command, start, *i - start))
		return (0);
	return (1);
}

int	token_analyser(char *command, t_tokens **my_tokens)
{
	int	i;

	i = 0;
	if (!command)
		return (0);
	while (command[i])
	{
		if (command[i] == ' ' || command[i] == '\t')
		{
			i++;
			continue ;
		}
		if (is_operator(command[i]))
		{
			if (!handel_operators(my_tokens, command, &i))
				return (free_tokens(*my_tokens));
		}
		else
		{
			if (!handle_word(my_tokens, command, &i))
				return (free_tokens(*my_tokens));
		}
	}
	return (1);
}
