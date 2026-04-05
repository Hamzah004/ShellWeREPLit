/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 18:24:15 by hbani-at          #+#    #+#             */
/*   Updated: 2026/03/29 19:06:31 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

static int	apply_redir_in(char *file_name);
static int	apply_redir_out_trunc(char *file_name);
static int	apply_redir_out_append(char *file_name);

int	apply_redir(t_commands *my_commands)
{
	t_redir			*redir;
	t_token_type	redir_type;

	redir = my_commands->redirection;
	while (redir)
	{
		redir_type = redir->type;
		if (redir_type == TOK_REDIR_IN)
			apply_redir_in(redir->file);
		if (redir_type == TOK_REDIR_OUT)
			apply_redir_out_trunc(redir->file);
		if (redir_type == TOK_APPEND)
			apply_redir_out_append(redir->file);
		redir = redir->next;
	}
	return (0);
}

static int	apply_redir_in(char *file_name)
{
	int	fd;

	fd = open(file_name, O_RDONLY);
	if (fd < 0)
	{
		perror("Error");
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	apply_redir_out_trunc(char *file_name)
{
	int	fd;

	fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
	{
		perror("Error");
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	apply_redir_out_append(char *file_name)
{
	int	fd;

	fd = open(file_name, O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (fd < 0)
	{
		perror("Error");
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}
