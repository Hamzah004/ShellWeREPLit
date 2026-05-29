/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 16:06:24 by hbani-at          #+#    #+#             */
/*   Updated: 2026/05/29 16:16:45 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"
#include <sys/wait.h>

// NOTE: you either make a pipe if needed or you don't so you set the pipe as -1
static void	make_pipe(t_commands *current, int pipe_fd[2])
{
	if (current->next)
		pipe(pipe_fd);
	else
	{
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
	}
}

static int	parent_cleanup(int pipe_fd[2], int prev_read)
{
	if (prev_read != -1)
		close(prev_read);
	if (pipe_fd[1] != -1)
		close(pipe_fd[1]);
	return (pipe_fd[0]);
}

void	wait_for_all(t_program_info *info, int last_pid)
{
	int	status;
	int	wait_reutrn;

	status = 0;
	wait_reutrn = waitpid(-1, &status, 0);
	while (wait_reutrn > 0)
	{
		if (wait_reutrn == last_pid)
			set_exit_from_status(info, status);
		wait_reutrn = waitpid(-1, &status, 0);
	}
}

void	pipeline_execution(t_program_info *info, t_commands *my_commands)
{
	int			pipe_fd[2];
	t_commands	*current;
	pid_t		pid;
	pid_t		last_pid;
	int			prev_read;

	current = my_commands;
	last_pid = -1;
	prev_read = -1;
	while (current)
	{
		make_pipe(current, pipe_fd);
		pid = fork();
		if (pid == 0)
			child_process(info, current, prev_read, pipe_fd);
		last_pid = pid;
		prev_read = parent_cleanup(pipe_fd, prev_read);
		current = current->next;
	}
	wait_for_all(info, last_pid);
}
