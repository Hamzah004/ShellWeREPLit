/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 14:27:38 by hbani-at          #+#    #+#             */
/*   Updated: 2026/05/09 14:58:43 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"
#include <limits.h>
#include <readline/readline.h>

// NOTE: this: acc * 10 + digit > ULLONG_MAX
// is same as this: if (acc > (ULLONG_MAX - digit) / 10)
// use it to check if there is a overflow

static int	parse_digit(const char *s, unsigned long long *num)
{
	int	digit;

	*num = 0;
	if (!*s)
		return (0);
	while (*s)
	{
		if (!ft_isdigit(*s))
			return (0);
		digit = *s - '0';
		if (*num > (ULLONG_MAX - digit) / 10)
			return (0);
		*num = *num * 10 + digit;
		s++;
	}
	return (1);
}

static int	validate_arg(const char *s, long long *out)
{
	int					i;
	int					sign;
	unsigned long long	num;

	if (!s || !*s)
		return (0);
	i = 0;
	sign = 1;
	if (s[i] == '+' || s[i] == '-')
	{
		if (s[i] == '-')
			sign = -1;
		i++;
	}
	if (!parse_digit(s + i, &num))
		return (0);
	if (sign == 1 && num > (unsigned long long)LLONG_MAX)
		return (0);
	if (sign == -1 && num > (unsigned long long)LLONG_MAX + 1ULL)
		return (0);
	if (sign == -1)
		*out = -(long long)num;
	else
		*out = (long long)num;
	return (1);
}

int	builtin_exit(t_program_info *info, char **argv)
{
	int			exit_status;
	long long	ll;

	ft_putendl_fd("exit", 2);
	if (!argv)
		return (1);
	if (!argv[1])
	{
		exit_status = info->exit_status;
		free_all_and_exit(info, (unsigned char)exit_status);
	}
	if (!validate_arg(argv[1], &ll))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		free_all_and_exit(info, 2);
	}
	if (argv[2])
	{
		ft_putendl_fd("minishell$ exit: too many arguments", 2);
		return (1);
	}
	free_all_and_exit(info, (unsigned char)ll);
	return (0);
}
