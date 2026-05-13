/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 09:41:13 by amufleh           #+#    #+#             */
/*   Updated: 2026/05/09 17:31:40 by amufleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"

int	input_validation(char *input)
{
	int		i;
	char	c;

	if (!input || !input[0])
		return (0);
	i = 0;
	c = 0;
	while (input[i])
	{
		if ((input[i] == '\'' || input[i] == '\"'))
		{
			if (c == 0)
				c = input[i];
			else if (c == input[i])
				c = 0;
		}
		i++;
	}
	if (c != 0)
		return (0);
	return (1);
}

int	yokotenkai(t_commands *cmd, t_envp *env, int exit_status)
{
	int		i;
	t_redir	*tmp;

	while (cmd)
	{
		i = 0;
		while (cmd->argv && cmd->argv[i])
		{
			if (!handle_expansion(cmd, env, exit_status, &i))
				return (0);
		}
		tmp = cmd->redirection;
		while (tmp)
		{
			if (tmp->type != TOK_HEREDOC)
			{
				tmp->file = replace_str(tmp->file);
				if (!tmp->file)
					return (0);
			}
			tmp = tmp->next;
		}
		cmd = cmd->next;
	}
	return (1);
}

t_commands	*fill_command_struct(char *input, t_envp *env, int exit_status)
{
	t_tokens	*my_tokens;
	t_commands	*my_commands;

	if (!input_validation(input))
		return (NULL);
	my_tokens = NULL;
	my_commands = new_command();
	if (!my_commands)
		return (NULL);
	token_analyser(input, &my_tokens);
	if (!syntax_validation(my_tokens))
		return (free_parser(my_commands, my_tokens));
	// print_tokens(my_tokens);
	if (!the_parser(my_tokens, my_commands))
		return (free_parser(my_commands, my_tokens));
	if (!yokotenkai(my_commands, env, exit_status))
		return (free_parser(my_commands, my_tokens));
	if (collect_heredocs(my_commands, env, exit_status) < 0)
		return (free_parser(my_commands, my_tokens));
	//remove_empty_arg(my_commands);
	free_tokens(my_tokens);
	return (my_commands);
}

// #include <readline/readline.h>
// int	main(int argc, char **argv, char **env)
// {
// 	argc = 0;
// 	argv = NULL;
// 	t_commands	*my_commands;
// 	//char	*input = "echo \'\'$HOME\'\'";
// 	char	*line;
// 	while ((line = readline("minishell$")) != NULL)
// 	{
// 		if (ft_strlen(line) > 0)
// 		{

// 			my_commands = fill_command_struct(line, env);
// 			if (my_commands)
// 			{
// 				print_commands(my_commands);
// 				free_commands(my_commands);
// 			}
// 		}
// 		free(line);
// 		line = (char *)NULL;
// 	}
// 	//printf("\n->%s", polish("       Abdallah        Almufleh     "));
// 	return (0);
// }
