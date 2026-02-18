/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 17:46:05 by amufleh           #+#    #+#             */
/*   Updated: 2026/02/18 16:12:01 by amufleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	add_redir(t_redir **redirection, t_tokens *token)
{
	t_redir	*redir;
	t_redir	*tmp;

	redir = new_redirection(token->type, token->next->value);
	if (!redir)
		return (0);
	if (!*redirection)
	{
		*redirection = redir;
		return (1);
	}
	tmp = *redirection;
	while (tmp->next)
	tmp = tmp->next;
	tmp->next = redir;
	return (1);
}

int	add_command(t_commands **command, int *size, t_tokens *tokens)
{
	t_commands *new_cmd;

	if (!command || !*command)
		return (0);
	new_cmd = new_command(tokens);
	if (!new_cmd)
		return (0);
	(*command)->next = new_cmd;
	new_cmd->prev = *command;
	*command = new_cmd;
	*size = 0;
	return (1);
}

int the_parser(t_tokens *my_tokens, t_commands *my_commands)
{
	t_tokens	*tokens;
	int			size;

	if (!syntax_validation(my_tokens))
		return (free_tokens(my_tokens));
	if (!my_commands)
		return (0);
	tokens = my_tokens;
	size = 0;
	while (tokens)
	{
		if (tokens->type == TOK_WORD)
		{
			if(!fill_command_options(my_commands, &size, tokens->value))
				return (0);
		}
		else if (is_redirection(tokens->type))
		{
			if (!add_redir(&my_commands->redirection, tokens))
				return (free_tokens(my_tokens));
			tokens = tokens->next->next;
			continue;
		}
		else if (tokens->type == TOK_PIPE)
		{
			if (!add_command(&my_commands, &size, tokens))
				return (0);
			tokens = tokens->next;
			continue;
		}
		tokens = tokens->next;
	}
	return (1);
}
