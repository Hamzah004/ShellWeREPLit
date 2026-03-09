/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 21:25:08 by hbani-at          #+#    #+#             */
/*   Updated: 2026/03/09 12:09:58 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <unistd.h>

int	count_lines(char **env)
{
	int	len;

	len = 0;
	while (env[len] != NULL)
	{
		len++;
	}
	return (len);
}

t_error	get_path(t_program_info *info)
{
	char	*path;
	int		i;

	i = 0;
	path = NULL;
	while (info->envp[i])
	{
		if (ft_strncmp(info->envp[i], "PATH=", 5) == 0)
		{
			path = ft_strdup(info->envp[i] + 5);
			// TEST: make sure to test if the check is working
			if (!path)
				return (ERROR_MEMORY);
			break ;
		}
		i++;
	}
	info->cmd_exec_dir = ft_split(path, ':');
	return (ERROR_SUCCESS);
}

char	*get_cmd_path(t_cmd *cmd, t_program_info *prog_info)
{
	char	*cmd_path;
	char	*tmp;
	int		i;

	i = 0;
	if (!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
		return (NULL);
	if (cmd->args[0][0] == '/' && access(cmd->args[0], X_OK) == 0)
		return (ft_strdup(cmd->args[0]));
	while (prog_info->cmd_exec_dir && prog_info->cmd_exec_dir[i])
	{
		tmp = ft_strjoin(prog_info->cmd_exec_dir[i], "/");
		// TODO: see if there is a return message will be here on the check
		if (!tmp)
			return (NULL);
		cmd_path = ft_strjoin(tmp, cmd->args[0]);
		free(tmp);
		if (!cmd_path)
			return (NULL);
		if (!access(cmd_path, X_OK))
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	return (NULL);
}

t_error	get_env(t_program_info *info, char **env)
{
	int	i;

	i = count_lines(env);
	info->envp = malloc(sizeof(char *) * (i + 1));
	if (!info->envp)
		return (ERROR_MEMORY);
	i = 0;
	while (env[i] != NULL)
	{
		info->envp[i] = ft_strdup(env[i]);
		i++;
	}
	return (ERROR_SUCCESS);
}

void	execute_single_cmd(t_cmd *cmd, t_program_info *info)
{
	char	*cmd_path;
	cmd_path = get_cmd_path(cmd, info);
	// char	*args[] = {cmd_path, "-la", NULL};

	execve(cmd_path, cmd->args, info->envp);
}
