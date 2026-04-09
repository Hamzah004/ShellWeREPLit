/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 16:43:25 by hbani-at          #+#    #+#             */
/*   Updated: 2026/04/09 20:41:43 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/execution.h"
#include "include/parsing.h"
#include "libft/libft.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static t_error	validate_arguments(int argc)
{
	if (argc > 1)
		return (ERROR_INVALID_ARGS);
	return (ERROR_SUCCESS);
}

void	read_from_prompt(t_program_info *info, char **env)
{
	char	*line;

	while ((line = readline("minishell$")) != NULL)
	{
		if (ft_strlen(line) > 0)
		{
			add_history(line);
			info->my_commands = fill_command_struct(line, env);
			print_commands(info->my_commands);
		}
		execution(info);
		free(line);
		line = (char *)NULL;
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
	info.old_stdin = -1;
	info.old_stdout = -1;
	info.cmd_exec_dir = NULL;
	volatile sig_atomic_t g_sig = 0;
	info.exit_status = 0;
	get_env(&info, env);
	get_path(&info);
	read_from_prompt(&info, info.envp);
	return (0);
}
