/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 16:18:49 by hbani-at          #+#    #+#             */
/*   Updated: 2026/05/08 04:01:19 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "parsing.h"
# include <signal.h>

extern volatile sig_atomic_t	g_sig;

typedef struct s_envp
{
	char						*key;
	char						*value;
	struct s_envp				*next;
}								t_envp;

typedef struct s_program_info
{
	int							exit_status;
	int							original_stdin;
	int							original_stdout;
	char						**envp;
	char						**cmd_exec_dir;
	t_commands					*my_commands;
	t_envp						*env;
}								t_program_info;

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
int								count_lines(char **env);
t_error							get_envp(t_program_info *info, char **env);
const char						*get_envp_value(char **envp, const char *name);
char							*get_cmd_bin(t_program_info *info);
void							execute_single_cmd(t_program_info *info);
void							read_from_prompt(t_program_info *info);
int								apply_redir(t_commands *my_commands);

/* main loop */
int								init_program_info(t_program_info *info,
									char **env);
void							destroy_program_info(t_program_info *info);
int								shell_loop(t_program_info *info);
int								is_blank_line(const char *line);

/* env */

int								get_and_update_path(t_program_info *info);
t_envp							*init_env(char **env);
void							env_add_back(t_envp **head, t_envp *new_node);
void							free_env(t_envp *head);
t_envp							*create_new_env_node(char *key, char *value);
int								env_unset(t_envp **head, const char *key);
t_envp							*env_find_node(t_envp *head, const char *key);
char							*env_find_value(t_envp *head, const char *key);
void							print_env(t_envp *head);
int								env_set(t_envp **head, const char *key,
									const char *value, int overwrite);
void							free_str_array(char **str);
char							**struct_to_arr(t_envp *env);

/* Signals */
void							setup_signals_interactive(void);

/* builtins */
int								builtin_cd(t_program_info *info);
int								builtin_unset(t_program_info *info,
									char **argv);
int								builtin_export(t_program_info *info,
									char **argv);
int								builtin_echo(t_program_info *info, char **argv);
int								builtin_pwd(t_program_info *info);
int								builtin_env(t_program_info *info, char **argv);
int								builtin_exit(t_program_info *info, char **argv);
int								is_valid_identifier(const char *s);

#endif
