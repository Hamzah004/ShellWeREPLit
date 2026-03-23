/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 16:21:16 by hbani-at          #+#    #+#             */
/*   Updated: 2026/02/11 16:22:17 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/execution.h"
#include "libft/libft.h"

void	print_error(t_error error)
{
	if (error == ERROR_SUCCESS)
		return ;
	ft_putstr_fd("Error\n", 2);
	if (error == ERROR_INVALID_ARGS)
		ft_putstr_fd("Invalid number of arguments\n", 2);
	if (error == ERROR_INVALID_PID)
		ft_putstr_fd("Invalid PID\n", 2);
	if (error == ERROR_INVALID_KILL)
		ft_putstr_fd("Failed to send signal\n", 2);
	if (error == ERROR_MEMORY)
		ft_putstr_fd("Memory allocation failed\n", 2);
}
