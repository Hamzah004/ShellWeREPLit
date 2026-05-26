/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 16:43:25 by hbani-at          #+#    #+#             */
/*   Updated: 2026/05/08 17:41:57 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/execution.h"
#include "include/parsing.h"
#include <readline/chardefs.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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

void	read_from_prompt(t_program_info *info)
{
	char	*line;

	while (1)
	{
		line = readline("minishell$ ");
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
		info->my_commands = fill_command_struct(line, info->env,
				info->exit_status);
		setup_signals_interactive();
		if (g_sig == SIGINT)
		{
			info->exit_status = 130;
			g_sig = 0;
		}
		if (!info->my_commands)
		{
			free(line);
			continue ;
		}
		// printf("command: %s\nargv[1]:%s\n", info->my_commands->argv[0], info->my_commands->argv[1]);
			execution(info);
			free_commands(info->my_commands);
			free(line);
			info->my_commands = NULL;
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
	read_from_prompt(&info);
	rl_clear_history();
	return (0);
}
