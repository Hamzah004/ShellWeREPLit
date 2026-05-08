/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 19:24:07 by hbani-at          #+#    #+#             */
/*   Updated: 2026/05/08 02:20:12 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"

int	is_valid_identifier(const char *s)
{
	if (!s || !*s)
		return (0);
	if (!ft_isalpha(*s) && *s != '_')
		return (0);
	s++;
	while (*s)
	{
		if (!ft_isalnum(*s) && *s != '_')
			return (0);
		s++;
	}
	return (1);
}

static int print_unset_error(char *arg)
{
	ft_putstr_fd("minishell: unset: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd("': not a valid identifier", 2);
	return (1);
}

int	builtin_unset(t_program_info *info, char **argv)
{
	int	status;
	if (!info || !argv)
		return (1);
	argv++;
	if (!*argv)
		return (0);
	status = 0;
	while (*argv)
	{
		if (is_valid_identifier(*argv))
			env_unset(&info->env, *argv);
		else
		{
			status = 1;
			print_unset_error(*argv);
		}
		argv++;
	}
	info->exit_status = status;
	return (status);
}
