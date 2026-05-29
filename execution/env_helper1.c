/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helper1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 00:37:20 by hbani-at          #+#    #+#             */
/*   Updated: 2026/05/29 15:49:28 by hbani-at         ###   ########.fr       */
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

static int	env_insert_new(t_envp **head, const char *key, const char *value)
{
	char	*new_key;
	char	*new_value;
	t_envp	*new_node;

	new_key = ft_strdup(key);
	new_value = NULL;
	if (value != NULL)
		new_value = ft_strdup(value);
	new_node = NULL;
	if (new_key && (!value || new_value))
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

static int	env_update(t_envp *node, const char *value)
{
	char	*tmp;

	if (value == NULL)
	{
		free(node->value);
		node->value = NULL;
		return (0);
	}
	tmp = ft_strdup(value);
	if (!tmp)
		return (1);
	free(node->value);
	node->value = tmp;
	return (0);
}

int	env_set(t_envp **head, const char *key, const char *value, int overwrite)
{
	t_envp	*node;

	if (!head || !key || key[0] == '\0')
		return (1);
	node = env_find_node(*head, key);
	if (!node)
		return (env_insert_new(head, key, value));
	if (overwrite == 0)
		return (0);
	return (env_update(node, value));
}
