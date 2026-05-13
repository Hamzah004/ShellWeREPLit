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

int	execution(t_program_info *info)
{
	t_commands	*my_commands;
	pid_t		pid;
	int			status;

	my_commands = info->my_commands;
	info->original_stdin = dup(STDIN_FILENO);
	info->original_stdout = dup(STDOUT_FILENO);
	while (my_commands)
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
		{
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
		dup2(info->original_stdin, STDIN_FILENO);
		dup2(info->original_stdout, STDOUT_FILENO);
		my_commands = my_commands->next;
	}
	close(info->original_stdin);
	close(info->original_stdout);
	return (0);
}
