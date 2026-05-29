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

char	*heredoc_expansion(char *input, t_program_info *info, int quoted)
{
	char	**tmp;
	char	*final_output;

	if (quoted)
	{
		final_output = ft_strdup(input);
		free(input);
		return (final_output);
	}
	tmp = expand_argv(input, info);
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

int	run_heredoc(t_redir *r, t_program_info *info)
{
	int		quoted;
	char	*delim;
	int		fd;

	quoted = is_quoted(r->file);
	delim = ft_strdup(r->file);
	if (!delim)
		return (-1);
	delim = replace_str(delim, info);
	if (!delim)
		return (-1);
	fd = handel_herdoc(delim, info, quoted);
	free(delim);
	return (fd);
}

int	collect_heredocs(t_commands *cmds, t_program_info *info)
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
				fd = run_heredoc(r, info);
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

int	heredoc_loop(t_heredoc *hd, t_program_info *info)
{
	char	*input;
	char	*after_exp;

	while (1)
	{
		input = readline("> ");
		if (g_sig == SIGINT || !input)
			return (heredoc_clean(hd, input));
		if (ft_strcmp(input, hd->delimiter) == 0)
			return (free(input), 0);
		after_exp = heredoc_expansion(input, info, hd->quoted);
		if (!after_exp)
			return (heredoc_clean(hd, NULL));
		add_free(after_exp, hd->fd[1]);
	}
}

int	handel_herdoc(char *drlimiter, t_program_info *info, int quoted)
{
	t_heredoc	hd;

	hd.delimiter = drlimiter;
	hd.quoted = quoted;
	if (heredoc_setup(&hd) == -1)
		return (-1);
	if (heredoc_loop(&hd, info) == -1)
		return (-1);
	close(hd.fd[1]);
	close(hd.save_stdin);
	return (hd.fd[0]);
}
