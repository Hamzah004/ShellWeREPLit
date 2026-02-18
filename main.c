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
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

static t_error	validate_arguments(int argc)
{
	if (argc > 1)
		return (ERROR_INVALID_ARGS);
	return (ERROR_SUCCESS);
}

int	main(int argc, char **argv, char **env)
{
	char	*line;
	t_error	err;

	(void)argv;
	(void)env;
	err = validate_arguments(argc);
	if (err != ERROR_SUCCESS)
	{
		print_error(err);
		return (err);
	}
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
	return (0);
}
