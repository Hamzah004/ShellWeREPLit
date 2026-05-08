/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 00:37:20 by hbani-at          #+#    #+#             */
/*   Updated: 2026/05/08 02:04:25 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"
#include <stdlib.h>
#include <string.h>

char	*env_find_value(t_envp *head, const char *key)
{
	if (!head || !key)
		return (NULL);
	while (head)
	{
		if (ft_strcmp(key, head->key) == 0)
			return (head->value);
		head = head->next;
	}
	return (NULL);
}

t_envp	*env_find_node(t_envp *head, const char *key)
{
	if (!head || !key)
		return (NULL);
	while (head)
	{
		if (ft_strcmp(key, head->key) == 0)
			return (head);
		head = head->next;
	}
	return (NULL);
}

int	env_set(t_envp **head, const char *key, const char *value, int overwrite)
{
	char	*new_key;
	char	*new_value;
	t_envp	*new_node;
	char	*tmp;

	if (!head || !key || key[0] == '\0')
		return (1);
	new_node = env_find_node(*head, key);
	if (!new_node)
	{
		new_key = ft_strdup(key);
		if (!new_key)
			return (1);
		if (value != NULL)
		{
			new_value = ft_strdup(value);
			if (!new_value)
			{
				free(new_key);
				return (1);
			}
		}
		else
			new_value = NULL;
		new_node = create_new_env_node(new_key, new_value);
		if (!new_node)
		{
			free(new_key);
			free(new_value);
			return (1);
		}
		env_add_back(head, new_node);
		return (0);
	}
	if (overwrite == 0)
		return (0);
	if (value == NULL)
	{
		free(new_node->value);
		new_node->value = NULL;
		return (0);
	}
	tmp = ft_strdup(value);
	if (!tmp)
		return (1);
	free(new_node->value);
	new_node->value = tmp;
	return (0);
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
