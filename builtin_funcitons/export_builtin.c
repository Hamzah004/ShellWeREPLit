/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 19:24:03 by hbani-at          #+#    #+#             */
/*   Updated: 2026/05/08 02:09:04 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"

static int	print_export_no_arg(t_envp *head)
{
	while (head)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(head->key, 1);
		if (head->value != NULL)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(head->value, 1);
			ft_putstr_fd("\"", 1);
		}
		ft_putstr_fd("\n", 1);
		head = head->next;
	}
	return (0);
}

static int	print_export_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd("': not a valid identifier", 2);
	return (1);
}

int	builtin_export(t_program_info *info, char **argv)
{
	int		status;
	char	*eq;
	char	*key;
	char	*value;

	if (!info || !argv)
		return (1);
	status = 0;
	argv++;
	if (!*argv)
	{
		print_export_no_arg(info->env);
		info->exit_status = 0;
		return (0);
	}
	while (*argv)
	{
		eq = ft_strchr(*argv, '=');
		if (eq)
		{
			key = ft_substr(*argv, 0, eq - *argv);
			value = ft_strdup(eq + 1);
			if (!key || (eq && !value))
			{
				free(key);
				free(value);
				info->exit_status = 1;
				return (1);
			}
			if (!is_valid_identifier(key))
			{
				print_export_error(*argv);
				status = 1;
			}
			else
			{
				env_set(&info->env, key, value, 1);
				if (!ft_strcmp(key, "PATH"))
					get_and_update_path(info);
			}
			free(key);
			free(value);
		}
		else
		{
			if (!is_valid_identifier(*argv))
			{
				print_export_error(*argv);
				status = 1;
			}
			else
				env_set(&info->env, *argv, NULL, 0);
		}
		argv++;
	}
	info->exit_status = status;
	return (status);
}
