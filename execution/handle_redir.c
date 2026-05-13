/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 18:24:15 by hbani-at          #+#    #+#             */
/*   Updated: 2026/04/05 14:41:46 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"
#include <errno.h>
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <unistd.h>

static int	apply_redir_in(char *file_name);
static int	apply_redir_out_trunc(char *file_name);
static int	apply_redir_out_append(char *file_name);
static int	apply_redir_heredoc(t_redir *redir);

int	apply_redir(t_commands *my_commands)
{
	int				status;
	t_redir			*redir;
	t_token_type	redir_type;

	redir = my_commands->redirection;
	status = 0;
	while (redir)
	{
		redir_type = redir->type;
		if (redir_type == TOK_HEREDOC)
			status = apply_redir_heredoc(redir);
		else if (redir_type == TOK_REDIR_IN)
			status = apply_redir_in(redir->file);
		else if (redir_type == TOK_REDIR_OUT)
			status = apply_redir_out_trunc(redir->file);
		else if (redir_type == TOK_APPEND)
			status = apply_redir_out_append(redir->file);
		if (status < 0)
			return (-1);
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
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(file_name, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		return (-1);
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
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(file_name, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		return (-1);
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
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(file_name, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	apply_redir_heredoc(t_redir *redir)
{
	if (redir->heredoc_fd != -1)
	{
		if (dup2(redir->heredoc_fd, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2");
			close(redir->heredoc_fd);
			return (-1);
		}
		close(redir->heredoc_fd);
		redir->heredoc_fd = -1;
	}
	return (0);
}
