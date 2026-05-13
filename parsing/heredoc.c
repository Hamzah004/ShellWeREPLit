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

#include "../include/parsing.h"

int		handel_herdoc(char *drlimiter, t_envp *env, int exit_status, int quoted);

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

char	*heredoc_expansion(char *input, t_envp *env, int exit_status, int quoted)
{
	char	**tmp;
	char	*final_output;

	if (!quoted)
	{
		final_output = ft_strdup(input);
		free (input);
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

int	handel_herdoc(char *drlimiter, t_envp *env, int exit_status, int quoted)
{
	char	*input;
	char	*after_exp;
	int		fd[2];

	if (pipe(fd) == -1)
		return (-1);
	while (1)
	{
		input = readline("> ");
		if (!input)
			return (-1);
		after_exp = heredoc_expansion(input, env, exit_status, quoted);
		if (!after_exp)
			return (-1);
		if (ft_strcmp(after_exp, drlimiter) == 0)
		{
			free(after_exp);
			break ;
		}
		add_free(after_exp, fd[1]);
	}
	close(fd[1]);
	return (fd[0]);
}
// int	main(int argc, char **argv, char **env)
// {
// 	argc = 0;
// 	argv = NULL;
// 	int		fd;
// 	char	buffer[1024];
// 	int		n;

// 	fd = handel_herdoc("EOF", env, 0);
// 	dup2(fd, 0);
// 	close(fd);
// 	n = read(0, buffer, sizeof(buffer) - 1);
// 	buffer[n] = '\0';
// 	printf("READ FROM STDIN:\n%s\n", buffer);
// 	return (0);
// }
