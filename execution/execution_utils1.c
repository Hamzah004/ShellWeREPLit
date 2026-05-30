/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 11:55:03 by hbani-at          #+#    #+#             */
/*   Updated: 2026/05/29 16:03:54 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"
#include <readline/history.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void	free_all_and_exit(t_program_info *info, int status)
{
	free_str_array(info->envp);
	free_str_array(info->cmd_exec_dir);
	free_env(info->env);
	free_commands(info->my_commands);
	if (info->original_stdin != -1)
		close(info->original_stdin);
	if (info->original_stdout != -1)
		close(info->original_stdout);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	rl_clear_history();
	exit(status);
}

static void	cmd_error(char *cmd, char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(msg, 2);
}

void	handle_no_cmd_path(t_program_info *info)
{
	char	*cmd;

	cmd = info->my_commands->argv[0];
	if (!ft_strchr(cmd, '/'))
	{
		cmd_error(cmd, ": command not found");
		free_all_and_exit(info, 127);
	}
	if (access(cmd, F_OK) != 0)
	{
		cmd_error(cmd, ": No such file or directory");
		free_all_and_exit(info, 127);
	}
	cmd_error(cmd, ": Permission denied");
	free_all_and_exit(info, 126);
}

void	set_exit_from_status(t_program_info *info, int status)
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
