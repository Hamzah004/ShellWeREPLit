/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 19:24:07 by hbani-at          #+#    #+#             */
/*   Updated: 2026/05/01 21:02:20 by hbani-at         ###   ########.fr       */
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

int	builtin_unset(t_program_info *info, char **arg)
{
	int	status;
	if (!info || !arg)
		return (1);
	arg++;
	if (!*arg)
		return (0);
	status = 0;
	while (*arg)
	{
		if (is_valid_identifier(*arg))
			env_unset(&info->env, *arg);
		else
		{
			status = 1;
			print_unset_error(*arg);
		}
		arg++;
	}
	info->exit_status = status;
	return (status);
}
