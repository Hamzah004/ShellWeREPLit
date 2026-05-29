/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 21:34:01 by hbani-at          #+#    #+#             */
/*   Updated: 2026/05/29 16:18:33 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char	*search_in_path(t_program_info *info)
{
	char	*cmd_bin;
	char	*tmp;
	int		i;

	i = 0;
	while (info->cmd_exec_dir && info->cmd_exec_dir[i])
	{
		tmp = ft_strjoin(info->cmd_exec_dir[i], "/");
		if (!tmp)
			return (NULL);
		cmd_bin = ft_strjoin(tmp, info->my_commands->argv[0]);
		free(tmp);
		if (!cmd_bin)
			return (NULL);
		if (!access(cmd_bin, X_OK))
			return (cmd_bin);
		free(cmd_bin);
		i++;
	}
	return (NULL);
}

char	*get_cmd_bin(t_program_info *info)
{
	if (!info->my_commands->argv || !info->my_commands->argv[0]
		|| info->my_commands->argv[0][0] == '\0')
		return (NULL);
	if (ft_strchr(info->my_commands->argv[0], '/'))
	{
		if (access(info->my_commands->argv[0], X_OK) == 0)
			return (ft_strdup(info->my_commands->argv[0]));
		return (NULL);
	}
	return (search_in_path(info));
}

int	get_and_update_path(t_program_info *info)
{
	const char	*path_value;

	if (!info)
		return (1);
	if (info->cmd_exec_dir)
		free_str_array(info->cmd_exec_dir);
	info->cmd_exec_dir = NULL;
	path_value = env_find_value(info->env, "PATH");
	if (!path_value || !*path_value)
		return (0);
	info->cmd_exec_dir = ft_split(path_value, ':');
	if (!info->cmd_exec_dir)
		return (1);
	return (0);
}

void	free_str_array(char **str)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}
