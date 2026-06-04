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

static void	heredoc_sigint(int signo)
{
	(void)signo;
	g_sig = SIGINT;
	close(STDIN_FILENO);
	write(1, "\n", 1);
}

void	setup_heredoc_signal(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = heredoc_sigint;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
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
