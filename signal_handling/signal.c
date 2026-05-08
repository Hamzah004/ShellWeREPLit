/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 01:57:23 by hbani-at          #+#    #+#             */
/*   Updated: 2026/04/20 16:57:44 by hbani-at         ###   ########.fr       */
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
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}
void	setup_signals_interactive(void)
{
	struct sigaction	sig_handler;
	sigemptyset(&sig_handler.sa_mask);
	sig_handler.sa_handler = sigint_interactive_handler;
	sig_handler.sa_flags = 0;
	sigaction(SIGINT, &sig_handler, NULL);
	signal(SIGQUIT, SIG_IGN);
}
