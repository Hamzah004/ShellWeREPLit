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
#include <signal.h>

volatile sig_atomic_t	g_sig = 0;

void	sig_handler_interactive(void);      
		// this is the handler called when we have a signal that needs to be handled
		// actual logic // will have all the handles
void	setup_signals_interactive(void); // the setup and call
void					setup_signals_child(void);
void					setup_signals_parent_wait(void);
void					consume_signal_result(t_program_info *info);

static void	sigint_interactive_handle(int sig, siginfo_t *info, void *val)
{
	(void)info;
	(void)val;
	if (sig == SIGINT)
		write(1, "\n", 1);
}

void	setup_signals_interactive(void)
{
	struct sigaction	handler;

	ft_bzero(&handler, sizeof(handler));
	handler.sa_flags = SA_SIGINFO;
	handler.sa_sigaction = &sigint_interactive_handle;
	sigaction(SIGINT, &handler, NULL);
	signal(SIGQUIT, SIG_DFL);
}

// - Goal
//   - Interactive mode:
//     - Ctrl-C: newline + fresh prompt, shell keeps running,
	// set last status to 130
//     - Ctrl-\: does nothing

//   - Execution mode:
//     - Children should react like normal processes (SIGINT/SIGQUIT default),
	// parent waits and records status
//  
	// - Respect subject rule: only one global signal variable storing signal number only.
// - Data Contract
//   - Keep global:
//     - volatile sig_atomic_t g_sig;
//   - No global structs, no global shell pointer.
// - Files / Functions
//   - signal_handling/signals.c
//     - void sig_handler_interactive(int signo);
//     - void setup_signals_interactive(void);
//     - void setup_signals_child(void);
//    
	// - void setup_signals_parent_wait(void); (optional if you want separate behavior while waiting)
//     - void consume_signal_result(t_program_info *info); (checks g_sig,
	// updates status/UI)
//   - include/execution.h
//     - prototypes + extern g_sig
//   - Makefile
//     - add signal_handling/signals.c to SRC
// - Behavior Details
//   - setup_signals_interactive:
//     - SIGINT => custom handler
//     - SIGQUIT => SIG_IGN
//   - handler:
//     - set g_sig = signo
//     - for SIGINT, write(1, "\n", 1) only (async-signal-safe)
//   - in prompt loop (after readline returns / before next prompt):
//     - if g_sig == SIGINT:
//       - info->exit_status = 130
//       - rl_on_new_line(); rl_replace_line("", 0); rl_redisplay();
//       - g_sig = 0
//   - child before execve:
//     - restore defaults with signal(SIGINT, SIG_DFL); signal(SIGQUIT,
	// SIG_DFL);
//   - parent wait status:
//     - if signaled: info->exit_status = 128 + signal
//     - if exited normally: WEXITSTATUS(status)
