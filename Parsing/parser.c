/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 17:46:05 by amufleh           #+#    #+#             */
/*   Updated: 2026/03/03 11:44:48 by amufleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	fill_command_options(t_commands *my_commands, int *size, char *token_value)
{
	char	*tmp;
	char	**new_argv;

	if (!my_commands || !size || !token_value)
		return (0);
	tmp = ft_strdup(token_value);
	if (!tmp)
		return (0);
	new_argv = ft_realloc(my_commands->argv, sizeof(char *) * (*size + 1),
			sizeof(char *) * (*size + 2));
	if (!new_argv)
	{
		free(tmp);
		return (0);
	}
	my_commands->argv = new_argv;
	my_commands->argv[*size] = tmp;
	(*size)++;
	my_commands->argv[*size] = NULL;
	return (1);
}

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

int	add_command(t_commands **command, int *size)
{
	t_commands	*new_cmd;

	if (!command || !*command)
		return (0);
	new_cmd = new_command();
	if (!new_cmd)
		return (0);
	(*command)->next = new_cmd;
	new_cmd->prev = *command;
	*command = new_cmd;
	*size = 0;
	return (1);
}

int	parse_token(t_commands **my_commands, t_tokens **token, int *size)
{
	if ((*token)->type == TOK_WORD)
	{
		if (!fill_command_options(*my_commands, size, (*token)->value))
			return (0);
		*token = (*token)->next;
		return (1);
	}
	else if (is_redirection((*token)->type))
	{
		if (!add_redir(&(*my_commands)->redirection, *token))
			return (0);
		*token = (*token)->next->next;
		return (1);
	}
	else if ((*token)->type == TOK_PIPE && !add_command(my_commands, size))
		return (0);
	*token = (*token)->next;
	return (1);
}

int	the_parser(t_tokens *my_tokens, t_commands *my_commands)
{
	t_tokens	*token;
	int			size;

	token = my_tokens;
	size = 0;
	while (token)
	{
		if (!parse_token(&my_commands, &token, &size))
			return (0);
	}
	return (1);
}
