/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 16:43:25 by hbani-at          #+#    #+#             */
/*   Updated: 2026/04/13 03:43:48 by hbani-at         ###   ########.fr       */
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


int	init_program_info(t_program_info *info, char **env)
{
	info->exit_status = 0;
	info->original_stdin = -1;
	info->original_stdout = -1;
	info->cmd_exec_dir = NULL;
	get_envp(info, env);
	get_and_update_path(info);
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

	while ((line = readline("minishell$")) != NULL)
	{
		if (ft_strlen(line) > 0)
		{
			add_history(line);
			info->my_commands = fill_command_struct(line, info->envp);
			if (!info->my_commands)
				continue ;
			// print_commands(info->my_commands);
			execution(info);
		}
		free_commands(info->my_commands);
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
	init_program_info(&info, env);
	read_from_prompt(&info);
	return (0);
}
