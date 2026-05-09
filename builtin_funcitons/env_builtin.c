/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 14:28:31 by hbani-at          #+#    #+#             */
/*   Updated: 2026/05/09 14:28:34 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"

int	builtin_env(t_program_info *info)
{
	t_envp	*current;

	if (!info)
		return (1);
	current = info->env;
	while (current)
	{
		if (current->value != NULL)
		{
			ft_putstr_fd(current->key, 1);
			ft_putchar_fd('=', 1);
			ft_putstr_fd(current->value, 1);
			ft_putchar_fd('\n', 1);
		}
		current = current->next;
	}
	return (0);
}
