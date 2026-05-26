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

int	builtin_unset(t_program_info *info, char **argv)
{
	if (!info || !argv)
		return (1);
	argv++;
	if (!*argv)
		return (0);
	while (*argv)
	{
		env_unset(&info->env, *argv);
		argv++;
	}
	return (0);
}
