/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 21:34:01 by hbani-at          #+#    #+#             */
/*   Updated: 2026/04/28 21:34:28 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"
#include <stdlib.h>
#include <string.h>

// t_env	*create_new_env_node(char *content);
// int		set_env_var(t_env **list, char *name, char *value);
// int		unset_env_var(t_env **list, char *name);
// t_env	*find_env_node(t_env *list, char *name);
// char	*get_env_var(t_env *list, char *name);
// int		is_valid_identifier(char *str);
// void	free_env_list(t_env *list);

void	free_str_array(char **cmd_exec_dir);

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

const char	*get_envp_value(char **envp, const char *name)
{
	int	i;
	int	name_len;

	if (!name || !*name || !envp)
		return (NULL);
	i = 0;
	name_len = ft_strlen(name);
	while (envp[i] != NULL)
	{
		if ((ft_strncmp(envp[i], name, name_len)) == 0
			&& envp[i][name_len] == '=')
			return (envp[i] + name_len + 1);
		i++;
	}
	return (NULL);
}

int	set_envp_value(char **envp, const char *name, const char *new_value)
{
	int	i;
	int	name_len;

	if (!name || !*name || !envp)
		return (1);
	i = 0;
	name_len = ft_strlen(name);
	while (envp[i] != NULL)
	{
		if ((ft_strncmp(envp[i], name, name_len)) == 0
			&& envp[i][name_len] == '=')
		{
			envp[i] = ft_strjoin(envp[i] + name_len + 1, new_value);
			return (0);
		}
		i++;
	}
	return (1);
}

// TODO: Update this function to take both absolute and rilative path
char	*get_cmd_bin(t_program_info *info)
{
	char	*cmd_bin;
	char	*tmp;
	int		i;

	i = 0;
	if (!info->my_commands->argv || !info->my_commands->argv[0]
		|| info->my_commands->argv[0][0] == '\0')
		return (NULL);
	if (info->my_commands->argv[0][0] == '/'
		&& access(info->my_commands->argv[0], X_OK) == 0)
		return (ft_strdup(info->my_commands->argv[0]));
	while (info->cmd_exec_dir && info->cmd_exec_dir[i])
	{
		tmp = ft_strjoin(info->cmd_exec_dir[i], "/");
		// TODO: see if there is a return message will be here on the check
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

void	free_str_array(char **cmd_exec_dir)
{
	int	i;
	int	lines_count;

	i = 0;
	lines_count = count_lines(cmd_exec_dir);
	while (i < lines_count)
	{
		if (cmd_exec_dir[i])
			free(cmd_exec_dir[i]);
		i++;
	}
	free(cmd_exec_dir);
}


t_error	get_envp(t_program_info *info, char **env)
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
	info->envp[i] = NULL;
	return (ERROR_SUCCESS);
}
