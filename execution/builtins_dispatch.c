/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_dispatch.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 16:02:49 by hbani-at          #+#    #+#             */
/*   Updated: 2026/05/29 16:11:32 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"
#include "../libft/libft.h"

int	isbuiltin(char *command)
{
	if (!ft_strcmp(command, "cd") || !ft_strcmp(command, "pwd")
		|| !ft_strcmp(command, "env") || !ft_strcmp(command, "export")
		|| !ft_strcmp(command, "echo") || !ft_strcmp(command, "unset")
		|| !ft_strcmp(command, "exit"))
		return (1);
	return (0);
}

int	execute_builtin(t_program_info *info)
{
	if (!ft_strcmp(info->my_commands->argv[0], "cd"))
		return (builtin_cd(info));
	else if (!ft_strcmp(info->my_commands->argv[0], "unset"))
		return (builtin_unset(info, info->my_commands->argv));
	else if (!ft_strcmp(info->my_commands->argv[0], "export"))
		return (builtin_export(info, info->my_commands->argv));
	else if (!ft_strcmp(info->my_commands->argv[0], "echo"))
		return (builtin_echo(info->my_commands->argv));
	else if (!ft_strcmp(info->my_commands->argv[0], "pwd"))
		return (builtin_pwd());
	else if (!ft_strcmp(info->my_commands->argv[0], "env"))
		return (builtin_env(info));
	else if (!ft_strcmp(info->my_commands->argv[0], "exit"))
		return (builtin_exit(info, info->my_commands->argv));
	return (0);
}
