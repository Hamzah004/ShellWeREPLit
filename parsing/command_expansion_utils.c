/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_expansion_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 13:05:42 by amufleh           #+#    #+#             */
/*   Updated: 2026/05/09 16:45:43 by amufleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"

char	*get_env_value(char *name, char **env)
{
	int		i;
	int		size;

	i = 0;
	size = ft_strlen(name);
	while (env[i])
	{
		if (ft_strncmp(env[i], name, size) == 0
			&& env[i][size] == '=')
		{
			return (ft_strdup(env[i] + size + 1));
		}
		i++;
	}
	return (ft_strdup(""));
}

char	*handle_env_value(char *name, char **env)
{
	char	*value;
	char	*after_polish;

	if (!name)
		return (NULL);
	value = get_env_value(name, env);
	if (!value)
		return (ft_strdup(""));
	after_polish = polish(value);
	free(value);
	if (!after_polish)
		return (NULL);
	return (after_polish);
}

char	*handle_var(char *input, t_track_quote *tracker, char **env)
{
	int		start;
	char	*name;
	char	*value;

	if (input[tracker->i] == '$' && input[tracker->i + 1] == '?')
	{
		tracker->i += 2;
		return (ft_strdup("$exit_state"));
	}
	tracker->i++;
	start = tracker->i;
	while (input[tracker->i] && (ft_isalnum(input[tracker->i])
			|| input[tracker->i] == '_'))
		tracker->i++;
	name = ft_strndup(input + start, tracker->i - start);
	value = handle_env_value(name, env);
	free(name);
	if (!value)
		return (ft_strdup(""));
	return (value);
}

char	*append(char *str, char c)
{
	int		i;
	char	*tmp;

	i = ft_strlen(str);
	tmp = ft_realloc(str, i, i + 2);
	if (!tmp)
		return (NULL);
	tmp[i] = c;
	tmp[i + 1] = '\0';
	return (tmp);
}

char	**handle_unquoted_var(char *tmp, char **result)
{
	char	**words;

	words = ft_split(tmp, ' ');
	free(tmp);
	if (!words)
		return (free_argv(result));
	if (count_words(words) > 1)
		result = insert_argv(result, words);
	else
	{
		if (words[0])
			result[0] = strjoin_free(result[0], words[0]);
		free_argv(words);
	}
	return (result);
}
