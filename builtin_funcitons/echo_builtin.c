/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/08 03:59:10 by hbani-at          #+#    #+#             */
/*   Updated: 2026/05/08 17:41:54 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"

static int	is_n_flag(char *s)
{
	int	i;
	if (!s || s[0] != '-' || s[1] != 'n')
		return (0);
	i = 2;
	while (s[i])
	{
		if (s[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_echo(char **argv)
{
	int	new_line;

	if (!argv)
		return (1);
	new_line = 1;
	argv++;
	while (*argv && is_n_flag(*argv))
	{
		new_line = 0;
		argv++;
	}
	while (*argv)
	{
		ft_putstr_fd(*argv, 1);
		if (argv[1] != NULL)
			ft_putchar_fd(' ', 1);
		argv++;
	}
	if (new_line)
		ft_putchar_fd('\n', 1);
	return (0);
}
