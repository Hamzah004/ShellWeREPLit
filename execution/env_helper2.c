/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helper2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 00:37:20 by hbani-at          #+#    #+#             */
/*   Updated: 2026/05/29 15:49:55 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"
#include <stdlib.h>
#include <string.h>

int	env_count_nodes(t_envp *env)
{
	int	counter;

	counter = 0;
	while (env)
	{
		if (env->value != NULL)
			counter++;
		env = env->next;
	}
	return (counter);
}

char	*env_make_line(const char *key, const char *value)
{
	char	*tmp;
	char	*line;

	if (!key || !value)
		return (NULL);
	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	line = ft_strjoin(tmp, value);
	free(tmp);
	return (line);
}

char	**struct_to_arr(t_envp *env)
{
	int		i;
	int		len;
	char	**envp;
	int		j;

	len = env_count_nodes(env);
	envp = malloc(sizeof(char *) * (len + 1));
	if (!envp)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->value != NULL)
		{
			envp[i] = env_make_line(env->key, env->value);
			if (!envp[i])
			{
				j = 0;
				while (j < i)
				{
					free(envp[j]);
					j++;
				}
				free(envp);
				return (NULL);
			}
			i++;
		}
		env = env->next;
	}
	envp[i] = NULL;
	return (envp);
}
