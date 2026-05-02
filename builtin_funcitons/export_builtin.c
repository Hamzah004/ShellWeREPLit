/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 19:24:03 by hbani-at          #+#    #+#             */
/*   Updated: 2026/05/01 19:24:10 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"

static void	print_export_no_arg(t_envp *head)
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
}

int builtin_export(t_program_info *info, char **argv)
{
	if (!info || !argv)
		return (1);
	argv++;
	if (!*argv)
		print_export_no_arg(info->env);
	return (0);
}
