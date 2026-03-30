/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 16:43:25 by hbani-at          #+#    #+#             */
/*   Updated: 2026/03/29 19:07:27 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/execution.h"
#include "include/parsing.h"
#include "libft/libft.h"
#include <bits/types/struct_itimerspec.h>
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

void	read_from_prompt(t_program_info *info)
{
	char	*line;

	while ((line = readline("minishell$")) != NULL)
	{
		if (ft_strlen(line) > 0)
		{
			add_history(line);
			info->my_commands = fill_command_struct(line);
			execution(info);
		}
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
	get_env(&info, env);
	get_path(&info);
	read_from_prompt(&info);
	return (0);
}
