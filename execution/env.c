/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 21:53:56 by hbani-at          #+#    #+#             */
/*   Updated: 2026/05/08 02:04:27 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"
#include <stdlib.h>

void	env_add_back(t_envp **head, t_envp *new_node)
{
	t_envp	*current_node;

	if (!head || !new_node)
		return ;
	if (!*head)
	{
		*head = new_node;
		return ;
	}
	current_node = *head;
	while (current_node->next)
		current_node = current_node->next;
	current_node->next = new_node;
}

void	free_env(t_envp *head)
{
	t_envp	*next;

	while (head)
	{
		next = head->next;
		free(head->key);
		free(head->value);
		free(head);
		head = next;
	}
}

t_envp	*create_new_env_node(char *key, char *value)
{
	t_envp	*new_node;

	new_node = malloc(sizeof(t_envp));
	if (!new_node)
		return (NULL);
	new_node->key = key;
	new_node->value = value;
	new_node->next = NULL;
	return (new_node);
}

t_envp	*init_env(char **env)
{
	t_envp	*head;
	t_envp	*new_node;
	char	*key;
	char	*value;
	char	*eq;
	int		key_len;

	if (!env)
		return (NULL);
	head = NULL;
	while (*env != NULL)
	{
		eq = ft_strchr(*env, '=');
		if (eq)
		{
			key_len = eq - *env;
			key = ft_substr(*env, 0, key_len);
			value = ft_strdup(eq + 1);
		}
		else
		{
			key = ft_strdup(*env);
			value = NULL;
		}
		if (!key || (eq && !value))
		{
			free(key);
			free(value);
			free_env(head);
			return (NULL);
		}
		new_node = create_new_env_node(key, value);
		if (!new_node)
		{
			free(key);
			free(value);
			free_env(head);
			return (NULL);
		}
		env_add_back(&head, new_node);
		env++;
	}
	return (head);
}
