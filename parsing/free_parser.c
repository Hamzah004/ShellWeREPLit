/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 10:08:58 by amufleh           #+#    #+#             */
/*   Updated: 2026/03/03 10:08:58 by amufleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"
#include <unistd.h>

char	**free_argv(char **argv)
{
	int	i;

	if (!argv)
		return (NULL);
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
	return (NULL);
}

void	free_redirections(t_redir *redir_list)
{
	t_redir	*tmp;

	while (redir_list)
	{
		tmp = redir_list->next;
		if (redir_list->heredoc_fd != -1)
			close(redir_list->heredoc_fd);
		if (redir_list->file)
			free(redir_list->file);
		free(redir_list);
		redir_list = tmp;
	}
}

void	free_commands(t_commands *my_commands)
{
	t_commands	*tmp;

	if (my_commands)
	{
		while (my_commands->prev)
			my_commands = my_commands->prev;
	}
	while (my_commands)
	{
		tmp = my_commands->next;
		if (my_commands->argv)
			free_argv(my_commands->argv);
		if (my_commands->redirection)
			free_redirections(my_commands->redirection);
		free(my_commands);
		my_commands = tmp;
	}
}

int	free_tokens(t_tokens *head)
{
	t_tokens	*tmp;

	while (head)
	{
		tmp = head->next;
		if (head->value)
			free(head->value);
		free(head);
		head = tmp;
	}
	return (0);
}

t_commands	*free_parser(t_commands *my_commands, t_tokens *my_tokens)
{
	free_tokens(my_tokens);
	free_commands(my_commands);
	return (NULL);
}
