/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 13:29:18 by hbani-at          #+#    #+#             */
/*   Updated: 2026/05/27 15:03:10 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"
#include "../include/parsing.h"
#include <signal.h>
#include <unistd.h>

void	add_free(char *after_exp, int fd)
{
	ft_putstr_fd(after_exp, fd);
	ft_putstr_fd("\n", fd);
	free(after_exp);
}

int	is_quoted(char *delimiter)
{
	int	len;

	if (!delimiter)
		return (0);
	len = ft_strlen(delimiter);
	if (len >= 2 && ((delimiter[0] == '\'' && delimiter[len - 1] == '\'')
			|| (delimiter[0] == '"' && delimiter[len - 1] == '"')))
		return (1);
	return (0);
}

int	heredoc_clean(t_heredoc *hd, char *input)
{
	free(input);
	dup2(hd->save_stdin, STDIN_FILENO);
	close(hd->save_stdin);
	close(hd->fd[0]);
	close(hd->fd[1]);
	return (-1);
}

int	heredoc_setup(t_heredoc *hd)
{
	if (pipe(hd->fd) == -1)
		return (-1);
	hd->save_stdin = dup(STDIN_FILENO);
	setup_heredoc_signal();
	return (0);
}

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
