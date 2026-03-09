/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugging_functions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 09:46:05 by hbani-at          #+#    #+#             */
/*   Updated: 2026/03/09 09:46:52 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"

void	print_tokens(t_tokens *head)
{
	t_tokens	*tmp;

	tmp = head;
	while (tmp)
	{
		printf("Token: [%s]  ", tmp->value);
		if (tmp->type == TOK_WORD)
			printf("WORD");
		else if (tmp->type == TOK_PIPE)
			printf("PIPE");
		else if (tmp->type == TOK_REDIR_IN)
			printf("REDIR_IN");
		else if (tmp->type == TOK_REDIR_OUT)
			printf("REDIR_OUT");
		else if (tmp->type == TOK_APPEND)
			printf("APPEND");
		else if (tmp->type == TOK_HEREDOC)
			printf("HEREDOC");
		else
			printf("UNKNOWN");
		printf("\n");
		tmp = tmp->next;
	}
}

void print_redirections(t_redir *redir)
{
	while (redir)
	{
		printf("  Redirection: type = ");
		if (redir->type == TOK_WORD)
			printf("WORD");
		else if (redir->type == TOK_PIPE)
			printf("PIPE");
		else if (redir->type == TOK_REDIR_IN)
			printf("REDIR_IN");
		else if (redir->type == TOK_REDIR_OUT)
			printf("REDIR_OUT");
		else if (redir->type == TOK_APPEND)
			printf("APPEND");
		else if (redir->type == TOK_HEREDOC)
			printf("HEREDOC");
		else
			printf("UNKNOWN");
		printf(", file = %s\n", redir->file);
		redir = redir->next;
	}
}

void print_commands(t_cmd *cmd)
{
	while (cmd)
	{

		if (cmd->args)
		{
			printf(" Arguments: ");
			for (int i = 0; cmd->args[i]; i++)
				printf("%s, ", cmd->args[i]);
			printf("\n");
		}
		else
		{
			printf(" Arguments: NULL\n");
		}
		if (cmd->redirection)
			print_redirections(cmd->redirection);
		else
			printf(" Redirections: NULL\n");
		printf("---------\n");
		cmd = cmd->next;
	}
}
