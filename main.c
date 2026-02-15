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
#include "libft/libft.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void	handle_ctrl_c(int signal)
{
	(void)signal;
	// if (signal == SIGINT)
	// {
	//
	// }
}

static t_error	validate_arguments(int argc)
{
	if (argc > 1)
		return (ERROR_INVALID_ARGS);
	return (ERROR_SUCCESS);
}

int	main(int argc, char **argv, char **env)
{
	char				*line;
	struct sigaction	sa;
	t_error				err;

	sa.sa_handler = &handle_ctrl_c;
	err = validate_arguments(argc);
	sigaction(SIGINT, &sa, NULL);
	if (err != ERROR_SUCCESS)
	{
		print_error(err);
		return (err);
	}
	while ((line = readline("minishell$")) != NULL)
	{
		if (strlen(line) > 0)
			add_history(line);
		if (!ft_strncmp(line, "clear", ft_strlen(line)))
		{
			execve("/usr/bin/clear", argv, env);
		}
		free(line);
	}
	return (0);
}
