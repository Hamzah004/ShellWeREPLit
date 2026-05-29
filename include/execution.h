/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 01:59:32 by hbani-at          #+#    #+#             */
/*   Updated: 2026/05/30 01:59:36 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "parsing.h"
# include <signal.h>

extern volatile sig_atomic_t	g_sig;

typedef enum e_error
{
	ERROR_SUCCESS = 0,
	ERROR_INVALID_ARGS = 1,
	ERROR_INVALID_PID = 2,
	ERROR_INVALID_KILL = 3,
	ERROR_MEMORY = 4,
	INVALID_UNSET_ARG = 5
}								t_error;

int								execution(t_program_info *info);
void							print_error(t_error error);
char							*get_cmd_bin(t_program_info *info);
void							execute_single_cmd(t_program_info *info);
void							read_from_prompt(t_program_info *info);
int								apply_redir(t_commands *my_commands);
void							free_all_and_exit(t_program_info *info,
									int status);
void							handle_no_cmd_path(t_program_info *info);
void							set_exit_from_status(t_program_info *info,
									int status);

/* main loop */

int								init_program_info(t_program_info *info,
									char **env);

/* env */

int								get_and_update_path(t_program_info *info);
t_envp							*init_env(char **env);
void							env_add_back(t_envp **head, t_envp *new_node);
void							free_env(t_envp *head);
t_envp							*create_new_env_node(char *key, char *value);
int								env_unset(t_envp **head, const char *key);
t_envp							*env_find_node(t_envp *head, const char *key);
char							*env_find_value(t_envp *head, const char *key);
int								env_set(t_envp **head, const char *key,
									const char *value, int overwrite);
void							free_str_array(char **str);
char							**struct_to_arr(t_envp *env);

/* Signals */

void							setup_signals_interactive(void);
void							setup_heredoc_signal(void);

/* builtins */

int								builtin_cd(t_program_info *info);
int								builtin_unset(t_program_info *info,
									char **argv);
int								builtin_export(t_program_info *info,
									char **argv);
int								builtin_echo(char **argv);
int								builtin_pwd(void);
int								builtin_env(t_program_info *info);
int								builtin_exit(t_program_info *info, char **argv);
int								is_valid_identifier(const char *s);
void							exec_builtin_and_single_cmd(t_program_info *info,
									t_commands *my_commands);
void							wait_for_all(t_program_info *info,
									int last_pid);
int								isbuiltin(char *command);
int								execute_builtin(t_program_info *info);
void							child_process(t_program_info *info,
									t_commands *current, int prev_read,
									int pipe_fd[2]);
void							pipeline_execution(t_program_info *info,
									t_commands *my_commands);
int								isbuiltin(char *command);
int								execute_builtin(t_program_info *info);
void							child_process(t_program_info *info, t_commands *current,
									int prev_read, int pipe_fd[2]);
void							pipeline_execution(t_program_info *info,
									t_commands *my_commands);

#endif
