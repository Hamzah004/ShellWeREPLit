/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 21:25:08 by hbani-at          #+#    #+#             */
/*   Updated: 2026/04/05 14:36:49 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

void	execute_single_cmd(t_program_info *info)
{
	char	*cmd_path;

	cmd_path = get_cmd_bin(info);
	if (cmd_path == NULL)
	{
		perror("command not found");
		exit(127);
	}
	execve(cmd_path, info->my_commands->argv, info->envp);
	perror("execve");
}

// int	execution(t_program_info *info)
// {
// 	int	pid;
//
// 	info->original_stdin = dup(STDIN_FILENO);
// 	info->original_stdout = dup(STDOUT_FILENO);
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		apply_redir(info->my_commands);
// 		execute_single_cmd(info);
// 	}
// 	waitpid(-1, NULL, 0);
// 	// parent process
// 	return (0);
// }

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
		builtin_cd(info);
	if (!ft_strcmp(info->my_commands->argv[0], "unset"))
		builtin_unset(info, info->my_commands->argv);
	return (1);
}

int	execution(t_program_info *info)
{
	int			pid;
	t_commands	*my_commands;

	info->original_stdin = dup(STDIN_FILENO);
	info->original_stdout = dup(STDOUT_FILENO);
	my_commands = info->my_commands;
	pid = fork();
	while (my_commands)
	{
		if (isbuiltin(my_commands->argv[0]))
			execute_builtin(info);
		else if (pid == 0)
		{
			// apply_redir(info->my_commands);
			execute_single_cmd(info);
		}
		waitpid(-1, NULL, 0);
		my_commands = my_commands->next;
	}
	return (0);
}
