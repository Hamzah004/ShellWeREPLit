/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_expansion_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 13:05:42 by amufleh           #+#    #+#             */
/*   Updated: 2026/04/05 13:28:18 by amufleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"

char	*push_char(char *s1, char s2)
{
	char	*new_str;
	size_t	i;

	if (!s1)
		return (NULL);
	i = 0;
	new_str = malloc((ft_strlen(s1) + 2) * sizeof(char));
	if (!new_str)
		return (NULL);
	while (i < ft_strlen(s1))
	{
		new_str[i] = s1[i];
		i++;
	}
	new_str[i] = s2;
	i++;
	new_str[i] = '\0';
	free(s1);
	return (new_str);
}

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

char	*handle_var(char *input, int *i, char **env)
{
	int		start;
	char	*name;
	char	*value;
	char	*after_polish;

	(*i)++;
	start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	name = ft_strndup(input + start, *i - start);
	if (!name)
		return (NULL);
	value = get_env_value(name, env);
	if (!value)
	{
		free(name);
		return (NULL);
	}
	free(name);
	after_polish = polish(value);
	free(value);
	if (!after_polish)
		return (NULL);
	return (after_polish);
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

char	*expand_files(char *file)
{
	int		i;
	char	*result;

	i = 0;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (file[i])
	{
		if (file[i] != '\'' && file[i] != '\"')
		{
			result = append(result, file[i]);
			if (!result)
				return (NULL);
		}
		i++;
	}
	return (result);
}
