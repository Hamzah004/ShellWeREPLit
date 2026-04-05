/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_expansion_utils2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 11:49:19 by amufleh           #+#    #+#             */
/*   Updated: 2026/04/05 13:28:26 by amufleh          ###   ########.fr       */
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
	if (!tmp_file)
		return (0);
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
			my_commands->argv[n] = my_commands->argv[i];
			n++;
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
