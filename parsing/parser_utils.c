/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 12:13:38 by amufleh           #+#    #+#             */
/*   Updated: 2026/04/05 13:28:44 by amufleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	copy_size;

	if (old_size < new_size)
		copy_size = old_size;
	else
		copy_size = new_size;
	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	if (ptr)
	{
		ft_memcpy(new_ptr, ptr, copy_size);
		free(ptr);
	}
	return (new_ptr);
}

t_redir	*new_redirection(t_token_type type, char *value)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = ft_strdup(value);
	if (!redir->file)
	{
		free(redir);
		return (NULL);
	}
	redir->next = NULL;
	return (redir);
}

t_commands	*new_command(void)
{
	t_commands	*command;

	command = malloc(sizeof(t_commands));
	if (!command)
		return (NULL);
	command->argv = malloc(sizeof(char *) * 1);
	if (!command->argv)
	{
		free(command);
		return (NULL);
	}
	command->argv[0] = NULL;
	command->redirection = NULL;
	command->next = NULL;
	command->prev = NULL;
	return (command);
}
