/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 16:43:25 by hbani-at          #+#    #+#             */
/*   Updated: 2026/06/04 23:47:34 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/execution.h"

int	init_program_info(t_program_info *info, char **env)
{
	info->envp = NULL;
	info->my_commands = NULL;
	info->exit_status = 0;
	info->original_stdin = -1;
	info->original_stdout = -1;
	info->cmd_exec_dir = NULL;
	info->env = init_env(env);
	get_and_update_path(info);
	setup_signals_interactive();
	return (0);
}

static t_error	validate_arguments(int argc)
{
	if (argc > 1)
		return (ERROR_INVALID_ARGS);
	return (ERROR_SUCCESS);
}

static int	process_cmd(t_program_info *info, char *line)
{
	info->my_commands = fill_command_struct(line, info);
	setup_signals_interactive();
	if (!info->my_commands)
	{
		if (g_sig == SIGINT)
			info->exit_status = 130;
		else
			info->exit_status = 2;
		g_sig = 0;
		free(line);
		return (1);
	}
	if (g_sig == SIGINT)
	{
		info->exit_status = 130;
		g_sig = 0;
	}
	execution(info);
	free_commands(info->my_commands);
	free(line);
	info->my_commands = NULL;
	return (0);
}

void	read_from_prompt(t_program_info *info)
{
	char	*line;
	// char	*prompt;

	while (1)
	{
		// prompt = readline_prompt(info);
		line = readline("ShellWeREPLit$ ");
		// free(prompt);
		if (g_sig == SIGINT)
		{
			info->exit_status = 130;
			g_sig = 0;
		}
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (line[0] == '\0')
		{
			free(line);
			continue ;
		}
		add_history(line);
		if (process_cmd(info, line) == 1)
			continue ;
	}
}

int	main(int argc, char **argv, char **env)
{
	t_program_info	info;
	t_error			err;

	(void)argv;
	err = validate_arguments(argc);
	if (err != ERROR_SUCCESS)
	{
		print_error(err);
		return (err);
	}
	init_program_info(&info, env);
	print_banner();
	read_from_prompt(&info);
	free_env(info.env);
	free_str_array(info.cmd_exec_dir);
	free_str_array(info.envp);
	rl_clear_history();
	return (info.exit_status);
}
