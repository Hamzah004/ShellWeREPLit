/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 01:59:32 by hbani-at          #+#    #+#             */
/*   Updated: 2026/06/04 23:40:59 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "parsing.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/chardefs.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>

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

/* ======================== ERROR ======================== */

# define GREEN "\033[1;32m"
# define YELLOW "\033[1;33m"
# define END_COLOR "\033[0m"
# define WHITE "\033[1;37m"
# define RED "\033[1;31m"
# define BLUE "\033[1;34m"
# define CYAN "\033[1;36m"
# define MAGENTA "\033[1;35m"
# define BLACK "\033[1;30m"
# define LIGHT_GRAY "\033[0;37m"
# define DARK_GRAY "\033[1;90m"
# define LIGHT_RED "\033[1;91m"
# define LIGHT_GREEN "\033[1;92m"
# define LIGHT_YELLOW "\033[1;93m"
# define LIGHT_BLUE "\033[1;94m"
# define LIGHT_MAGENTA "\033[1;95m"
# define LIGHT_CYAN "\033[1;96m"

void	print_banner(void);
char   *readline_prompt(t_program_info *info);

/* ======================== ERROR ======================== */

void							print_error(t_error error);

/* ====================== EXECUTION ====================== */

int								execution(t_program_info *info);
char							*get_cmd_bin(t_program_info *info);
void							execute_single_cmd(t_program_info *info);
int								apply_redir(t_commands *my_commands);
void							handle_no_cmd_path(t_program_info *info);
void							builtin_and_single_cmd(t_program_info *info,
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

/* ====================== MAIN LOOP ====================== */

int								init_program_info(t_program_info *info,
									char **env);
void							read_from_prompt(t_program_info *info);
void							free_all_and_exit(t_program_info *info,
									int status);
void							set_exit_from_status(t_program_info *info,
									int status);

/* ===================== ENVIRONMENT ===================== */

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

/* ======================= SIGNALS ======================= */

void							setup_signals_interactive(void);
void							setup_heredoc_signal(void);

/* ======================= BUILTINS ====================== */

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

#endif
