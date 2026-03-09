/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 12:13:38 by amufleh           #+#    #+#             */
/*   Updated: 2026/02/18 16:25:28 by amufleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"
#include "../libft/libft.h"

t_redir	*new_redirection(t_token_type type, char *value)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = value;
	redir->next = NULL;
	return (redir);
}

int	count_words(t_tokens *token)
{
	t_tokens *tmp;
	int	count;

	count = 0;
	tmp = token;
	while(tmp && tmp->type != TOK_PIPE)
	{
		if (tmp->type == TOK_WORD)
			count++;
		tmp = tmp -> next;
	}
	return (count);
}
t_cmd	*new_command(t_tokens *tokens)
{
	t_cmd	*command;
	int	size;

	size = count_words(tokens);
	command = malloc(sizeof(t_cmd));
	if (!command)
		return (NULL);
	command->args =  malloc(sizeof(char *) * (size + 2));
	if (!command->args)
		return (NULL);
	command->redirection = NULL;
	command->next = NULL;
	command->prev = NULL;

	return (command);
}

int	fill_command_options(t_cmd *my_commands, int *size, char *tokens_value)
{
	if (!my_commands)
		return (0);
	my_commands->args[*size] = ft_strdup(tokens_value);
	(*size)++;
	my_commands->args[*size] = NULL;
	return (1);
}

int	free_commands(t_cmd *cmd)
{
	t_cmd	*tmp;
	t_redir	*redir_tmp;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->args)
		{
			int i = 0;
			while (cmd->args[i])
			{
				free(cmd->args[i]);
				i++;
			}
			free(cmd->args);
		}
		while (cmd->redirection)
		{
			redir_tmp = cmd->redirection->next;
			free(cmd->redirection);
			cmd->redirection = redir_tmp;
		}
		free(cmd);
		cmd = tmp;
	}
	return (1);
}
