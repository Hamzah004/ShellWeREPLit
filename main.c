/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 16:43:25 by hbani-at          #+#    #+#             */
/*   Updated: 2026/03/09 08:59:25 by hbani-at         ###   ########.fr       */
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
			// NOTE: testing only
			/*if (!info -> my_commands)
				free_parser(info->my_commands)*/
			print_commands(info->my_commands);
		}
		free(line);
		line = (char *)NULL;
	}
}

int	main(int argc, char **argv, char **env)
{
	t_program_info	info;
	t_error			err;
	int				pid;

	(void)argv;
	(void)env;
	(void)pid;
	err = validate_arguments(argc);
	if (err != ERROR_SUCCESS)
	{
		print_error(err);
		return (err);
	}
	read_from_prompt(&info);
	return (0);
}
