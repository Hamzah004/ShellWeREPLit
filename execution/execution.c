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
		perror("command not found");
		exit(127);
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
		execute_single_cmd(info);
		exit(0);
	}
	else
		waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		info->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		info->exit_status = 128 + WTERMSIG(status);
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

static void	pipeline_execution(t_program_info *info, t_commands *my_commands)
{
	t_commands	*current;
	pid_t	pid;
	pid_t	last_pid;

	current = my_commands;
	while (current)
	{
		pid = fork();
		if (pid == 0)
			exec_builtin_and_single_cmd(info, my_commands);
		else
			last_pid = pid;
		current = current->next;
	}
}

int	execution(t_program_info *info)
{
	int	count;

	info->original_stdin = dup(STDIN_FILENO);
	info->original_stdout = dup(STDOUT_FILENO);
	count = commands_count(info->my_commands);
	if (count == 1)
		exec_builtin_and_single_cmd(info, info->my_commands);
	else
		pipeline_execution(info, info->my_commands);
	close(info->original_stdin);
	close(info->original_stdout);
	return (0);
}
