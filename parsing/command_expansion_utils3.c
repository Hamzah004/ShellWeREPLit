/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_expansion_utils3.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 11:27:23 by amufleh           #+#    #+#             */
/*   Updated: 2026/05/09 16:45:37 by amufleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"

int	count_words(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
		i++;
	return (i);
}

char	**insert_argv(char **argv, char **words)
{
	int		new_words;
	char	**new_argv;
	int		i;
	int		j;

	i = 1;
	j = 0;
	new_words = count_words(words);
	new_argv = malloc(sizeof(char *) * (new_words + 2));
	if (!new_argv)
		return (NULL);
	new_argv[0] = argv[0];
	while (j < new_words)
	{
		new_argv[i++] = words[j++];
	}
	new_argv[i] = NULL;
	free(argv);
	free(words);
	return (new_argv);
}

char	**insert_words(char **argv, int index, char **words)
{
	int		old_size;
	int		new_size;
	char	**new_argv;

	old_size = count_words(argv);
	new_size = count_words(words);
	new_argv = malloc(sizeof(char *) * (old_size - 1 + new_size + 1));
	if (!new_argv)
		return (NULL);
	if (index > 0)
		ft_memcpy(new_argv, argv, sizeof(char *) * index);
	ft_memcpy(new_argv + index, words, sizeof(char *) * new_size);
	if (old_size - index - 1 > 0)
		ft_memcpy(new_argv + index + new_size, argv + index + 1,
			sizeof(char *) * (old_size - index - 1));
	new_argv[old_size - 1 + new_size] = NULL;
	free(argv[index]);
	free(argv);
	return (new_argv);
}

char	*strjoin_free(char *old, char *new)
{
	char	*final;

	final = ft_strjoin(old, new);
	if (!final)
	{
		free(old);
		return (NULL);
	}
	free(old);
	return (final);
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
	if (!result[0])
	{
		free(result);
		return (NULL);
	}
	return (result);
}
