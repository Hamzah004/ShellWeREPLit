/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 19:24:03 by hbani-at          #+#    #+#             */
/*   Updated: 2026/05/08 02:17:46 by hbani-at         ###   ########.fr       */
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

static int	export_assign(t_program_info *info, char *argv, char *eq)
{
	int		status;
	char	*key;
	char	*value;

	key = ft_substr(argv, 0, eq - argv);
	value = ft_strdup(eq + 1);
	status = 0;
	if (!key || !value)
		status = 1;
	else if (!is_valid_identifier(key))
		status = print_export_error(argv);
	else
	{
		env_set(&info->env, key, value, 1);
		if (!ft_strcmp(key, "PATH"))
			get_and_update_path(info);
	}
	free(key);
	free(value);
	return (status);
}

static int	export_one(t_program_info *info, char *argv)
{
	char	*eq;

	eq = ft_strchr(argv, '=');
	if (eq)
		return (export_assign(info, argv, eq));
	if (!is_valid_identifier(argv))
		return (print_export_error(argv));
	env_set(&info->env, argv, NULL, 0);
	return (0);
}

int	builtin_export(t_program_info *info, char **argv)
{
	int	status;

	if (!info || !argv)
		return (1);
	argv++;
	if (!*argv)
		return (print_export_no_arg(info->env));
	status = 0;
	while (*argv)
	{
		if (export_one(info, *argv) != 0)
			status = 1;
		argv++;
	}
	return (status);
}
