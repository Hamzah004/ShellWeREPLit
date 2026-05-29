/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 16:56:51 by hbani-at          #+#    #+#             */
/*   Updated: 2026/04/20 16:57:38 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"
#include <errno.h>
#include <unistd.h>

const char	*get_path(t_program_info *info)
{
	const char	*path;

	if (info->my_commands->argv[1] && info->my_commands->argv[2])
	{
		ft_putendl_fd("minishell: cd: too many arguments", 2);
		return (NULL);
	}
	if (!info->my_commands->argv[1])
		path = env_find_value(info->env, "HOME");
	else
		path = info->my_commands->argv[1];
	return (path);
}

int	builtin_cd(t_program_info *info)
{
	const char	*path;
	char		old[1024];
	char		new[1024];

	if (!info)
		return (1);
	path = get_path(info);
	if (!path)
		return (1);
	getcwd(old, sizeof(old));
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd((char *)path, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		return (1);
	}
	getcwd(new, sizeof(new));
	env_set(&info->env, "OLDPWD", old, 1);
	env_set(&info->env, "PWD", new, 1);
	return (0);
}
