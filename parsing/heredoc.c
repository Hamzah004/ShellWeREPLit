/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 14:18:10 by amufleh           #+#    #+#             */
/*   Updated: 2026/05/27 15:09:13 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"
#include "../include/parsing.h"
#include <signal.h>
#include <unistd.h>
 
char	*heredoc_expansion(char *input, t_envp *env, int exit_status,
		int quoted)
{
	char	**tmp;
	char	*final_output;
 
	if (quoted)
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
 
int	run_heredoc(t_redir *r, t_envp *env, int exit_status)
{
	int		quoted;
	char	*delim;
	int		fd;
 
	quoted = is_quoted(r->file);
	delim = ft_strdup(r->file);
	if (!delim)
		return (-1);
	delim = replace_str(delim);
	if (!delim)
		return (-1);
	fd = handel_herdoc(delim, env, exit_status, quoted);
	free(delim);
	return (fd);
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
				fd = run_heredoc(r, env, exit_status);
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

int	heredoc_loop(char *drlimiter, t_envp *env, int exit_status,
				int quoted, int fd[2], int save_stdin)
{
	char	*input;
	char	*after_exp;

	while (1)
	{
		input = readline("> ");
		if (g_sig == SIGINT || !input)
			return (heredoc_clean(fd, save_stdin, input));
		if (ft_strcmp(input, drlimiter) == 0)
			return (free(input), 0);
		after_exp = heredoc_expansion(input, env, exit_status, quoted);
		if (!after_exp)
			return (heredoc_clean(fd, save_stdin, NULL));
		add_free(after_exp, fd[1]);
	}
}

int	handel_herdoc(char *drlimiter, t_envp *env, int exit_status, int quoted)
{
	int	fd[2];
	int	save_stdin;

	if (heredoc_setup(fd, &save_stdin) == -1)
		return (-1);
	if (heredoc_loop(drlimiter, env, exit_status, quoted, fd, save_stdin) == -1)
		return (-1);
	close(fd[1]);
	close(save_stdin);
	return (fd[0]);
}