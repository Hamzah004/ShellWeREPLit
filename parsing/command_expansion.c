/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 13:04:59 by amufleh           #+#    #+#             */
/*   Updated: 2026/05/09 16:45:33 by amufleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"

char	**handel_single_quote(char *input, int *i, char **result)
{
	if (!input)
		return (NULL);
	if (input[*i] == '\'' || input[*i] == '\"')
	{
		(*i)++;
		return (result);
	}
	result[0] = append(result[0], input[*i]);
	if (!result)
		return (NULL);
	(*i)++;
	return (result);
}

char	**handel_double_quote(t_track_quote *tracker, char *input,
		char **result, t_envp *env, int exit_status)
{
	char	*tmp;

	tmp = handle_var(input, tracker, env, exit_status);
	if (!tmp)
		return (free_argv(result));
	if (tracker->d_quote)
	{
		result[0] = strjoin_free(result[0], tmp);
		free(tmp);
		if (!result[0])
			return (free_argv(result));
		return (result);
	}
	return (handle_unquoted_var(tmp, result));
}

char	**quote_tracker(t_track_quote *tracker, char *input,
	char **result, t_envp *env, int exit_status)
{
	if (input[tracker->i] == '\'')
	{
		tracker->s_quote = !tracker->s_quote;
		tracker->was_quoted = 1;
		tracker->i++;
	}
	else if (input[tracker->i] == '\"')
	{
		tracker->d_quote = !tracker->d_quote;
		tracker->was_quoted = 1;
		tracker->i++;
	}
	if (input[tracker->i] == '$' && !tracker->s_quote)
	{
		result = handel_double_quote(tracker, input, result, env, exit_status);
		if (!result)
			return (NULL);
	}
	else if (input[tracker->i] && input[tracker->i] != '\'')
	{
		result = handel_single_quote(input, &tracker->i, result);
		if (!result)
			return (NULL);
	}
	return (result);
}

char	**expand_argv(char *input, t_envp *env, int exit_status)
{
	char			**result;
	t_track_quote	tracker;

	tracker.i = 0;
	tracker.d_quote = 0;
	tracker.s_quote = 0;
	tracker.was_quoted = 0;
	result = malloc(sizeof(char *) * 2);
	if (!result)
		return (NULL);
	result[0] = ft_strdup("");
	result[1] = NULL;
	while (input[tracker.i])
	{
		result = quote_tracker(&tracker, input, result, env, exit_status);
		if (!result)
			return (NULL);
	}
	if (!result[1] && result[0] && !result[0][0] && tracker.was_quoted)
	{
		free(result[0]);
		result[0] = ft_strdup("");
	}
	return (result);
}

int	handle_expansion(t_commands *cmd, t_envp *env, int exit_status, int *i)
{
	char	**tmp;
	int		added_words;

	tmp = expand_argv(cmd->argv[*i], env, exit_status);
	if (!tmp)
		return (0);
	if (tmp[1])
	{
		added_words = count_words(tmp);
		cmd->argv = insert_words(cmd->argv, *i, tmp);
		*i += added_words;
		free(tmp);
	}
	else
	{
		free(cmd->argv[*i]);
		cmd->argv[*i] = tmp[0];
		free(tmp);
		(*i)++;
	}
	return (1);
}
