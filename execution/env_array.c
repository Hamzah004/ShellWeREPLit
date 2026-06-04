/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_array.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 00:37:20 by hbani-at          #+#    #+#             */
/*   Updated: 2026/06/04 23:30:45 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"

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

static void	free_partial(char **envp, int count)
{
	int	j;

	j = 0;
	while (j < count)
	{
		free(envp[j]);
		j++;
	}
	free(envp);
}

char	**struct_to_arr(t_envp *env)
{
	int		i;
	int		len;
	char	**envp;

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
				free_partial(envp, i);
				return (NULL);
			}
			i++;
		}
		env = env->next;
	}
	envp[i] = NULL;
	return (envp);
}

int	env_unset(t_envp **head, const char *key)
{
	t_envp	*current;
	t_envp	*prev;

	if (!head || !key || !key[0])
		return (1);
	current = *head;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev == NULL)
				*head = current->next;
			else
				prev->next = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}
