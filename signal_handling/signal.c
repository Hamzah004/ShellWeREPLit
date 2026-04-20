/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 01:57:23 by hbani-at          #+#    #+#             */
/*   Updated: 2026/04/13 03:43:43 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/execution.h"
#include <readline/readline.h>
#include <signal.h>

volatile sig_atomic_t	g_sig = 0;

static void	sigint_interactive_handler(int signo)
{
	(void)signo;
	g_sig = SIGINT;
	rl_done = 1;
	write(1, "\n", 1);
}
void	setup_signals_interactive(void)
{
	struct sigaction	sa;rl_replace_line,
	// rl_catch_signals = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = sigint_interactive_handler;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	// signal(SIGQUIT, SIG_IGN);
}
