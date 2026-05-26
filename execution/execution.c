/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 21:25:08 by hbani-at          #+#    #+#             */
/*   Updated: 2026/05/08 17:42:01 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void	execute_single_cmd(t_program_info *info)
{
	char	*cmd_path;

	if (info->envp)
		free_str_array(info->envp);
	info->envp = struct_to_arr(info->env);
	if (!info->envp)
	{
		perror("env allocation failed");
		exit(1);
	}
	cmd_path = get_cmd_bin(info);
	if (cmd_path == NULL)
	{
		if (ft_strchr(info->my_commands->argv[0], '/'))
		{
			if (access(info->my_commands->argv[0], F_OK) != 0)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(info->my_commands->argv[0], 2);
				ft_putendl_fd(": No such file or directory", 2);
				exit(127);
			}
			if (access(info->my_commands->argv[0], X_OK) != 0)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(info->my_commands->argv[0], 2);
				ft_putendl_fd(": Permission denied", 2);
				exit(126);
			}
		}
		else
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(info->my_commands->argv[0], 2);
			ft_putendl_fd(": command not found", 2);
			exit(127);
		}
	}
	execve(cmd_path, info->my_commands->argv, info->envp);
	perror("execve");
}

static int	isbuiltin(char *command)
{
	if (!ft_strcmp(command, "cd") || !ft_strcmp(command, "pwd")
		|| !ft_strcmp(command, "env") || !ft_strcmp(command, "export")
		|| !ft_strcmp(command, "echo") || !ft_strcmp(command, "unset")
		|| !ft_strcmp(command, "exit"))
		return (1);
	return (0);
}

static int	execute_builtin(t_program_info *info)
{
	if (!ft_strcmp(info->my_commands->argv[0], "cd"))
		return (builtin_cd(info));
	else if (!ft_strcmp(info->my_commands->argv[0], "unset"))
		return (builtin_unset(info, info->my_commands->argv));
	else if (!ft_strcmp(info->my_commands->argv[0], "export"))
		return (builtin_export(info, info->my_commands->argv));
	else if (!ft_strcmp(info->my_commands->argv[0], "echo"))
		return (builtin_echo(info->my_commands->argv));
	else if (!ft_strcmp(info->my_commands->argv[0], "pwd"))
		return (builtin_pwd());
	else if (!ft_strcmp(info->my_commands->argv[0], "env"))
		return (builtin_env(info));
	else if (!ft_strcmp(info->my_commands->argv[0], "exit"))
		return (builtin_exit(info, info->my_commands->argv));
	return (0);
}

static int	commands_count(t_commands *my_commands)
{
	int	count;

	count = 0;
	while (my_commands)
	{
		my_commands = my_commands->next;
		count++;
	}
	return (count);
}

static void	exec_external_cmd(t_program_info *info, t_commands *my_commands)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		if (apply_redir(my_commands) < 0)
			exit(1);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		close(info->original_stdin);
		close(info->original_stdout);
		execute_single_cmd(info);
		exit(0);
	}
	else
		waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		info->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", 2);
		info->exit_status = 128 + WTERMSIG(status);
	}
}

static void	exec_builtin_and_single_cmd(t_program_info *info,
		t_commands *my_commands)
{
	if (!my_commands->argv || !my_commands->argv[0])
	{
		if (apply_redir(my_commands) < 0)
			info->exit_status = 1;
		else
			info->exit_status = 0;
	}
	else if (isbuiltin(my_commands->argv[0]))
	{
		if (apply_redir(my_commands) < 0)
			info->exit_status = 1;
		else
			info->exit_status = execute_builtin(info);
	}
	else
		exec_external_cmd(info, my_commands);
	dup2(info->original_stdin, STDIN_FILENO);
	dup2(info->original_stdout, STDOUT_FILENO);
}

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
void	set_child_fds(int pipe_fd[2], int prev_read)
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
	set_child_fds(pipe_fd, prev_read);
	info->my_commands = current;
	if (apply_redir(current) < 0)
		exit(1);
	if (!current->argv || !current->argv[0])
		exit(0);
	if (isbuiltin(current->argv[0]))
		exit(execute_builtin(info));
	execute_single_cmd(info);
	exit(1);
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

	wait_reutrn = waitpid(-1, &status, 0);
	while (wait_reutrn > 0)
	{
		if (wait_reutrn == last_pid)
		{
			if (WIFEXITED(status))
				info->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				if (WTERMSIG(status) == SIGINT)
					write(1, "\n", 1);
				else if (WTERMSIG(status) == SIGQUIT)
					ft_putendl_fd("Quit (core dumped)", 2);
				info->exit_status = 128 + WTERMSIG(status);
			}
		}
		wait_reutrn = waitpid(-1, &status, 0);
	}
}

static void	pipeline_execution(t_program_info *info, t_commands *my_commands)
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

int	execution(t_program_info *info)
{
	int	count;

	info->original_stdin = dup(STDIN_FILENO);
	info->original_stdout = dup(STDOUT_FILENO);
	signal(SIGINT, SIG_IGN);
	count = commands_count(info->my_commands);
	if (count == 1)
		exec_builtin_and_single_cmd(info, info->my_commands);
	else
		pipeline_execution(info, info->my_commands);
	setup_signals_interactive();
	close(info->original_stdin);
	close(info->original_stdout);
	return (0);
}
