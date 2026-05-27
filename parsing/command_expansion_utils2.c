/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_expansion_utils2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 11:49:19 by amufleh           #+#    #+#             */
/*   Updated: 2026/05/27 14:03:26 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"

char	*polish(char *value)
{
	int		i;
	char	*result;

	i = 0;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (value[i] == ' ')
		i++;
	while (value[i])
	{
		if (value[i] == ' ' && value[i + 1] == ' ')
		{
			i++;
			continue ;
		}
		result = append(result, value[i]);
		if (!result)
			return (NULL);
		i++;
	}
	return (result);
}

char	*replace_str(char *old)
{
	char	*tmp_file;

	tmp_file = expand_files(old);
	free(old);
	return (tmp_file);
}

int	only_space(char *str)
{
	if (!str || !str[0])
		return (1);
	else
		return (0);
}

int	remove_empty_arg(t_commands *my_commands)
{
	int	i;
	int	n;

	i = 0;
	n = 0;
	while (my_commands->argv[i])
	{
		if (!only_space(my_commands->argv[i]))
		{
			if (my_commands->argv[i][0] == '\0')
			{
				free(my_commands->argv[i]);
				continue ;
			}
			my_commands->argv[n++] = my_commands->argv[i];
		}
		else
		{
			free(my_commands->argv[i]);
		}
		i++;
	}
	my_commands->argv[n] = NULL;
	return (1);
}

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
