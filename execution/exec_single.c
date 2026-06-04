/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 16:11:56 by hbani-at          #+#    #+#             */
/*   Updated: 2026/05/29 16:13:59 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"

static void	exec_external_cmd(t_program_info *info, t_commands *my_commands)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		info->exit_status = 1;
		return ;
	}
	if (pid == 0)
	{
		if (apply_redir(my_commands) < 0)
			free_all_and_exit(info, 1);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		close(info->original_stdin);
		close(info->original_stdout);
		info->original_stdin = -1;
		info->original_stdout = -1;
		execute_single_cmd(info);
	}
	waitpid(pid, &status, 0);
	set_exit_from_status(info, status);
}

void	builtin_and_single_cmd(t_program_info *info,
		t_commands *my_commands)
{
	if (my_commands->argv && my_commands->argv[0]
		&& !isbuiltin(my_commands->argv[0]))
		return (exec_external_cmd(info, my_commands));
	if (apply_redir(my_commands) < 0)
		info->exit_status = 1;
	else if (!my_commands->argv || !my_commands->argv[0])
		info->exit_status = 0;
	else
		info->exit_status = execute_builtin(info);
	if (my_commands->redirection)
	{
		dup2(info->original_stdin, STDIN_FILENO);
		dup2(info->original_stdout, STDOUT_FILENO);
	}
}

void	execute_single_cmd(t_program_info *info)
{
	char	*cmd_path;

	if (info->envp)
		free_str_array(info->envp);
	info->envp = struct_to_arr(info->env);
	if (!info->envp)
	{
		perror("env allocation failed");
		free_all_and_exit(info, 1);
	}
	cmd_path = get_cmd_bin(info);
	if (cmd_path == NULL)
		handle_no_cmd_path(info);
	execve(cmd_path, info->my_commands->argv, info->envp);
	perror("execve");
	free(cmd_path);
	free_all_and_exit(info, 126);
}
