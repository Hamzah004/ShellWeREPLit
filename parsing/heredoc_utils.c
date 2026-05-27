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

#include "../libft/libft.h"

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
	if (len >= 2
		&& ((delimiter[0] == '\'' && delimiter[len - 1] == '\'')
			|| (delimiter[0] == '"' && delimiter[len - 1] == '"')))
		return (1);
	return (0);
}

void	close_pipe(int fd[2])
{
	close(fd[0]);
	close(fd[1]);
}
