/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 16:18:49 by hbani-at          #+#    #+#             */
/*   Updated: 2026/02/11 16:24:00 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "parsing.h"

typedef enum e_error
{
	ERROR_SUCCESS = 0,
	ERROR_INVALID_ARGS = 1,
	ERROR_INVALID_PID = 2,
	ERROR_INVALID_KILL = 3,
	ERROR_MEMORY = 4
}					t_error;

typedef struct s_program_info
{
	int				old_stdin;
	int				old_stdout;
	char			**envp;
	t_commands		*my_commands;
	char			**cmd_exec_dir;
}					t_program_info;

typedef struct s_env
{
	char			*content;
	struct s_env	*next;
}					t_env;

int					execution(t_program_info *info);
void				print_error(t_error error);
int					count_lines(char **env);
t_error				get_env(t_program_info *info, char **env);
t_error				get_path(t_program_info *info);
void				execute_single_cmd(t_commands *my_commands,
						t_program_info *info);

#endif
