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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <readline/history.h>
#include <readline/readline.h>

int	main(int argc, char **argv, char **env)
{
	struct sigaction	sa;
	char	*buf;

	while ((buf = readline("minishell$")) != NULL)
	{
		if (strlen(buf) > 0)
		{
			add_history(buf);
		}
		printf("You typed: %s\n", buf);
		free(buf);
	}
	return (0);
}
