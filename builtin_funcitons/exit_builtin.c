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
#include <readline/readline.h>
#include <limits.h>

// NOTE: this: acc * 10 + digit > ULLONG_MAX
// is same as this: if (acc > (ULLONG_MAX - digit) / 10)
// use it to check if there is a overflow

static int	validate_arg(const char *s, long long *out)
{
	int					i;
	int					sign;
	unsigned long long	num;
	int					digit;

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
	if (!s[i])
		return (0);
	num = 0;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		digit = s[i] - '0';
		if (num > (ULLONG_MAX - digit) / 10)
			return (0);
		num = num * 10 + digit;
		i++;
	}
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

static void	kill_this_mada_faka(t_program_info *info, int status)
{
	free_env(info->env);
	free_str_array(info->envp);
	free_str_array(info->cmd_exec_dir);
	free_commands(info->my_commands);
	rl_clear_history();
	exit(status);
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
		kill_this_mada_faka(info, (unsigned char)exit_status);
	}
	if (!validate_arg(argv[1], &ll))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		kill_this_mada_faka(info, 2);
	}
	if (argv[2])
	{
		ft_putendl_fd("minishell$ exit: too many arguments", 2);
		// TODO: check the status code and if you kill or not in campus
		return (1);
	}
	kill_this_mada_faka(info, (unsigned char)ll);
	return (0);
}
