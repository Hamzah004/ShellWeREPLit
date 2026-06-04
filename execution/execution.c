/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 21:25:08 by hbani-at          #+#    #+#             */
/*   Updated: 2026/05/29 16:20:36 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"

static int	commands_count(t_commands *my_commands)
{
	int	count;

	count = 0;
	while (my_commands)
	{
		my_commands = my_commands->next;
		count++;
	}
	return (count);
}

int	execution(t_program_info *info)
{
	int	count;

	info->original_stdin = dup(STDIN_FILENO);
	info->original_stdout = dup(STDOUT_FILENO);
	signal(SIGINT, SIG_IGN);
	count = commands_count(info->my_commands);
	if (count == 1)
		builtin_and_single_cmd(info, info->my_commands);
	else
		pipeline_execution(info, info->my_commands);
	setup_signals_interactive();
	close(info->original_stdin);
	close(info->original_stdout);
	return (0);
}
