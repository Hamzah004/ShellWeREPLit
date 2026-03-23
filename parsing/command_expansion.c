/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 13:04:59 by amufleh           #+#    #+#             */
/*   Updated: 2026/03/10 15:26:35 by amufleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"

char	*push_char(char const *s1, char s2)
{
	char	*new_str;
	size_t	i;

	if (!s1 || !s2)
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
	return (new_str);
}

int	get_vname(char *str, int start)
{
	int	i;

	i = start;
	while (str[i])
	{
		if (str[i] == '$' || str[i] == '\'' || str[i] == '\'')
			break ;
		i++;
	}
	return (i);
}

void	double_quotes(char *str, int start)
{
	char	*tmp;
	int		i;
	int		j;
	int		size;

	i = start;
	while(str[i] != '\"' && str[i])
	{
		if (str[i] == '$')
		{
			j = get_vname(str, i);
			tmp = ft_substr(str, i, j);
			size = ft_strlen(str);
			if ((i + j) < size)
				i += j;
			else
				break ;
		}
		i++;
	}
	printf("%s ", tmp);
}

int	command_expansion(t_commands *my_commands)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (my_commands->argv[i])
	{
		j = 0;
		while (my_commands->argv[i][j])
		{
			if (my_commands->argv[i][j] == '\"')
			{
				j++;
				double_quotes(my_commands->argv[i], j);
				break ;
			}
		}
		// printf("%s  ", tmp);
		// tmp = ft_strdup("\0");
		i++;
	}
	return (0);
}
// if not virabel
// 	remove '' and ""
// else
// 	cheak if it must be replaced or not
// 	and modfay it

