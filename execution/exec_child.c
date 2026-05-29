/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 16:10:07 by hbani-at          #+#    #+#             */
/*   Updated: 2026/05/29 16:17:04 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"

static void	set_child_fds(int pipe_fd[2], int prev_read)
{
	if (prev_read != -1)
	{
		dup2(prev_read, STDIN_FILENO);
		close(prev_read);
	}
	if (pipe_fd[1] != -1)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	if (pipe_fd[0] != -1)
		close(pipe_fd[0]);
}

void	child_process(t_program_info *info, t_commands *current, int prev_read,
		int pipe_fd[2])
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(info->original_stdin);
	close(info->original_stdout);
	info->original_stdin = -1;
	info->original_stdout = -1;
	set_child_fds(pipe_fd, prev_read);
	info->my_commands = current;
	if (apply_redir(current) < 0)
		free_all_and_exit(info, 1);
	if (!current->argv || !current->argv[0])
		free_all_and_exit(info, 0);
	if (isbuiltin(current->argv[0]))
		free_all_and_exit(info, execute_builtin(info));
	execute_single_cmd(info);
}
