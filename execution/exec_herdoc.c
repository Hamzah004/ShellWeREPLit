/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_herdoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 16:14:32 by hbani-at          #+#    #+#             */
/*   Updated: 2026/04/05 17:56:05 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"
#include <readline/readline.h>
#include <unistd.h>

// int	apply_redir_heredoc(t_program_info *info)
// {
// 	int	fd[2];
// 	char	*line;
//
// 	if (pipe(fd) == -1)
// 		return (1);
// 	while ((line = readline("")) != NULL)
// 	{
// 		if (ft_strlen(line) > 0)
// 		{
// 			write(fd[1], &line, strlen(line) - 1);
// 			if (ft_strcmp(line, info->my_commands->redirection->file))
// 				break ;
// 			dup2(fd[0], STDIN_FILENO);
// 		}
// 	}
// 		return (0);
// }

// handling herdoc
// what is the logic
// we will receive herdoc type from the redir enum
// if heredoc do the logic
// start by make a pipe[2] which are the two ends of the pipe
// use readline to read the line entered
// take the line and write it using write system call to pipin
// and this will be the input for the cmd that will be executed if it take a input like cat,
// grep, cut, sort
