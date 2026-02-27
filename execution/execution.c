/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 21:25:08 by hbani-at          #+#    #+#             */
/*   Updated: 2026/02/18 21:35:46 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

char	*get_path(char **envp)
{
	char	*path;
	char	**path_vals;
	int		i;

	i = 0;
	path = NULL;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = ft_strdup(envp[i] + 5);
			if (!path)
				return (NULL);
			break ;
		}
		i++;
	}
	return (path);
}

char	*get_cmd_path(t_cmd *cmd, t_program_info *prog_info)
{
	char	*str;
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
		if (!tmp)
			return (NULL);
		str = ft_strjoin(tmp, cmd->args[0]);
		free(tmp);
		if (!str)
			return (NULL);
		if (!access(str, X_OK))
			return (str);
		free(str);
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

void	execute_single_cmd(t_program_info *info)
{
	// execve
	// error handle
}
