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

#include "GNL/get_next_line.h"
#include "include/minishell.h"
#include "include/parsing.h"
#include "libft/libft.h"
#include <bits/types/struct_itimerspec.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

static t_error	validate_arguments(int argc)
{
	if (argc > 1)
		return (ERROR_INVALID_ARGS);
	return (ERROR_SUCCESS);
}

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

char	*get_cmd_path(t_cmd *info)
{
	char	*str;
	char	*tmp;
	int		i;

	i = 0;
	if (!info->args || !info->args[0]
		|| info->args[0][0] == '\0')
		return (NULL);
	if (info->args[0][0] == '/' &&
		access(info->args[0], X_OK) == 0)
		return (ft_strdup(info->args[0]));
	while (info->command_folders[i])
	{
		tmp = ft_strjoin(info->command_folders[i], "/");
		if (!tmp)
			return (NULL);
		str = ft_strjoin(tmp, info->args[0]);
		free(tmp);
		if (!access(str, X_OK))
			return (str);
		free(str);
		i++;
	}
	return (NULL);
}

void	execute_single_cmd(t_program_info *info)
{
	// execve
	// error handle
}

void	get_env(t_program_info *info, char **env)
{
	int	i;

	i = count_lines(env);
	info->envp = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (env[i] != NULL)
	{
		info->envp[i] = ft_strdup(env[i]);
		i++;
	}
}

void	read_from_prompt(void)
{
	char	*line;

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
}

int	main(int argc, char **argv, char **env)
{
	t_program_info	info;
	t_error			err;
	int				pid;

	(void)argv;
	err = validate_arguments(argc);
	if (err != ERROR_SUCCESS)
	{
		print_error(err);
		return (err);
	}
	get_env(&info, env);
	read_from_prompt();
	info.cmd = malloc(sizeof(*info.cmd));
	pid = fork();
	if (pid == 0)
		execute_single_cmd(&info);
	return (0);
}
