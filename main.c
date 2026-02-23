/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 16:43:25 by hbani-at          #+#    #+#             */
/*   Updated: 2026/02/11 16:44:13 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"
#include "GNL/get_next_line.h"
#include "include/parsing.h"
#include "libft/libft.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

static t_error	validate_arguments(int argc)
{
	if (argc > 1)
		return (ERROR_INVALID_ARGS);
	return (ERROR_SUCCESS);
}

// void	execute_single_cmd(void)
// {
// 	execve(, char *const *argv, char *const *envp)
// }

void	get_env(char **env)
{

}

void	read_from_prompt(void)
{
	char	*line;

	while ((line = readline("minishell$")) != NULL)
	{
		if (ft_strlen(line) > 0)
		{
			if (line && *line)
				add_history(line);
		}
		if (line)
		{
			free(line);
			line = (char *)NULL;
		}
	}
}

int	main(int argc, char **argv, char **env)
{
	t_error	err;

	// t_cmd	cmd;
	// int	pid;
	(void)argv;
	err = validate_arguments(argc);
	if (err != ERROR_SUCCESS)
	{
		print_error(err);
		return (err);
	}
	get_env(env);
	while (*env)
	{
		printf("%s\n", *env);
		if (env++ == NULL)
			break ;
	}
	// read_from_prompt();
	// pid = fork();
	// if (pid == 0)
	// 	execute_single_cmd();
	return (0);
}
