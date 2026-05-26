/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 14:18:10 by amufleh           #+#    #+#             */
/*   Updated: 2026/05/09 16:45:13 by amufleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"
#include "../include/parsing.h"
#include <signal.h>
#include <unistd.h>

int			handel_herdoc(char *drlimiter, t_envp *env, int exit_status,
				int quoted);

char	*handel_multyarg(char **tmp)
{
	int		i;
	char	*output;
	char	*new_output;

	output = ft_strdup("");
	if (!output)
		return (NULL);
	i = 0;
	while (tmp[i])
	{
		new_output = ft_strjoin(output, tmp[i]);
		free(output);
		if (!new_output)
			return (NULL);
		output = new_output;
		i++;
	}
	return (output);
}

char	*heredoc_expansion(char *input, t_envp *env, int exit_status,
		int quoted)
{
	char	**tmp;
	char	*final_output;

	if (!quoted)
	{
		final_output = ft_strdup(input);
		free(input);
		return (final_output);
	}
	tmp = expand_argv(input, env, exit_status);
	free(input);
	if (!tmp)
		return (0);
	if (tmp[1])
	{
		final_output = handel_multyarg(tmp);
		free_argv(tmp);
		return (final_output);
	}
	else
	{
		final_output = ft_strdup(tmp[0]);
		free_argv(tmp);
		return (final_output);
	}
}

void	add_free(char *after_exp, int fd)
{
	ft_putstr_fd(after_exp, fd);
	ft_putstr_fd("\n", fd);
	free(after_exp);
}

int	collect_heredocs(t_commands *cmds, t_envp *env, int exit_status)
{
	t_redir	*r;
	int		fd;

	while (cmds)
	{
		r = cmds->redirection;
		while (r)
		{
			if (r->type == TOK_HEREDOC && r->heredoc_fd == -1)
			{
				fd = handel_herdoc(r->file, env, exit_status, 1);
				if (fd < 0)
					return (-1);
				r->heredoc_fd = fd;
			}
			r = r->next;
		}
		cmds = cmds->next;
	}
	return (0);
}

static int	heredoc_clean(int fd[2], int save_stdin, char *input)
{
	free(input);
	dup2(save_stdin, STDIN_FILENO);
	close(save_stdin);
	close(fd[0]);
	close(fd[1]);
	return (-1);
}

static int	heredoc_setup(int fd[2], int *save_stdin)
{
	if (pipe(fd) == -1)
		return (-1);
	*save_stdin = dup(STDIN_FILENO);
	setup_heredoc_signal();
	return (0);
}

int	handel_herdoc(char *drlimiter, t_envp *env, int exit_status, int quoted)
{
	char	*input;
	char	*after_exp;
	int		fd[2];
	int		save_stdin;

	if (heredoc_setup(fd, &save_stdin) == -1)
		return (-1);
	while (1)
	{
		input = readline("> ");
		if (g_sig == SIGINT)
			return (heredoc_clean(fd, save_stdin, input));
		if (!input)
			return (heredoc_clean(fd, save_stdin, input));
		after_exp = heredoc_expansion(input, env, exit_status, quoted);
		if (!after_exp)
			return (heredoc_clean(fd, save_stdin, NULL));
		if (ft_strcmp(after_exp, drlimiter) == 0)
		{
			free(after_exp);
			break ;
		}
		add_free(after_exp, fd[1]);
	}
	close(fd[1]);
	close(save_stdin);
	return (fd[0]);
}
