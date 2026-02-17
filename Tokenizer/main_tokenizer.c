/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_tokenizer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 09:41:13 by amufleh           #+#    #+#             */
/*   Updated: 2026/02/14 13:59:10 by amufleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

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

int main()
{
	t_tokens *my_tokens = NULL;
	char *input = "<< \'l\'s -la || < grep a >> \"\'$HOME\'\" infile.txt echo \"abdallah\" -n";
	token_analyser(input, &my_tokens);
	print_tokens(my_tokens);
	free_tokens(my_tokens);
	return (0);
}
