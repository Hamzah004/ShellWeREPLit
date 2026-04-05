/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 13:04:59 by amufleh           #+#    #+#             */
/*   Updated: 2026/04/05 13:38:40 by amufleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"

char	*handel_double_quote(char *input, int *i, char *result, char **env)
{
	char	*tmp;
	char	*new_result;

	if (!input)
		return (NULL);
	tmp = handle_var(input, i, env);
	if (!tmp)
	{
		free(result);
		return (NULL);
	}
	new_result = ft_strjoin(result, tmp);
	free(tmp);
	if (!new_result)
	{
		free(result);
		return (NULL);
	}
	free(result);
	return (new_result);
}

char	*handel_single_quote(char *input, int *i, char *result)
{
	if (!input)
		return (NULL);
	result = append(result, input[*i]);
	if (!result)
		return (NULL);
	(*i)++;
	return (result);
}

char	*quote_tracker(t_track_quote *tracker, char *input,
	char *result, char **env)
{
	if (input[tracker->i] == '\'' && !tracker->d_quote)
	{
		tracker->s_quote = !tracker->s_quote;
		tracker->i++;
	}
	else if (input[tracker->i] == '\"' && !tracker->s_quote)
	{
		tracker->d_quote = !tracker->d_quote;
		tracker->i++;
	}
	if (input[tracker->i] == '$' && !tracker->s_quote)
	{
		result = handel_double_quote(input, &tracker->i, result, env);
		if (!result)
			return (NULL);
	}
	else if (input[tracker->i])
	{
		result = handel_single_quote(input, &tracker->i, result);
		if (!result)
			return (NULL);
	}
	return (result);
}

char	*expand_argv(char *input, char **env)
{
	char			*result;
	t_track_quote	tracker;

	tracker.i = 0;
	tracker.d_quote = 0;
	tracker.s_quote = 0;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (input[tracker.i])
	{
		result = quote_tracker(&tracker, input, result, env);
		if (!result)
			return (NULL);
	}
	return (result);
}

int	yokotenkai(t_commands *my_commands, char **env)
{
	int		i;
	char	*tmp;

	while (my_commands)
	{
		i = 0;
		while (my_commands->argv && my_commands->argv[i])
		{
			tmp = expand_argv(my_commands->argv[i], env);
			if (!tmp)
				return (0);
			free(my_commands->argv[i]);
			my_commands->argv[i] = tmp;
			i++;
		}
		if (my_commands->redirection && my_commands->redirection->file)
		{
			my_commands->redirection->file
				= replace_str(my_commands->redirection->file);
		}
		my_commands = my_commands->next;
	}
	return (1);
}
