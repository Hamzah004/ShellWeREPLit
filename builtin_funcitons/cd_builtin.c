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
#include <unistd.h>

const char	*get_path(t_program_info *info)
{
	const char	*path;

	if (info->my_commands->argv[1] && info->my_commands->argv[2])
	{
		perror("Error");
		return (NULL);
	}
	if (!info->my_commands->argv[1])
		path = get_env_value("HOME", info->envp);
	else
		path = info->my_commands->argv[1];
	return (path);
}

// void	update_env_vars(t_program_info *info)
// {
//
// }

int	execute_cd_builtin(t_program_info *info)
{
	const char	*path;

	// char	cwd[1024];
	if (!info)
		return (1);
	path = get_path(info);
	if (!path)
		return (1);
	chdir(path);
	// getcwd(cwd, sizeof(cwd));
	return (0);
}

// what to do now:
// Step 1 (spec lock): write a tiny cd behavior contract before coding (5-8 lines in your notes),
	// with these rules:
//  - cd <relative_or_absolute_path>: try chdir(path), return 0 on success.
//  - cd with no arg: use $HOME (bash reference).
//  - cd with more than one arg: print error, return 1.
//  - On chdir failure: print perror("minishell: cd"), return 1.
//  - On success: update OLDPWD to previous cwd and PWD to new cwd.
