/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 09:41:13 by amufleh           #+#    #+#             */
/*   Updated: 2026/05/27 15:09:02 by hbani-at         ###   ########.fr       */
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

int	yokotenkai(t_commands *cmd, t_program_info *info)
{
	int		i;
	t_redir	*tmp;

	while (cmd)
	{
		i = 0;
		while (cmd->argv && cmd->argv[i])
		{
			if (!handle_expansion(cmd, info, &i))
				return (0);
		}
		tmp = cmd->redirection;
		while (tmp)
		{
			if (tmp->type != TOK_HEREDOC)
			{
				tmp->file = replace_str(tmp->file, info);
				if (!tmp->file)
					return (0);
			}
			tmp = tmp->next;
		}
		cmd = cmd->next;
	}
	return (1);
}

t_commands	*fill_command_struct(char *input, t_program_info *info)
{
	t_tokens	*my_tokens;
	t_commands	*my_commands;

	if (!input_validation(input))
	{
		ft_putendl_fd("-minishell: syntax error unclosed quotes", 2);
		return (NULL);
	}
	my_tokens = NULL;
	my_commands = new_command();
	if (!my_commands)
		return (NULL);
	token_analyser(input, &my_tokens);
	if (!syntax_validation(my_tokens))
		return (free_parser(my_commands, my_tokens));
	if (!the_parser(my_tokens, my_commands))
		return (free_parser(my_commands, my_tokens));
	if (!yokotenkai(my_commands, info))
		return (free_parser(my_commands, my_tokens));
	if (collect_heredocs(my_commands, info) < 0)
		return (free_parser(my_commands, my_tokens));
	remove_empty_arg(my_commands);
	free_tokens(my_tokens);
	return (my_commands);
}
