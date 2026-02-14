/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validatio.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 15:30:47 by amufleh           #+#    #+#             */
/*   Updated: 2026/02/14 15:30:50 by amufleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

int	is_redirection (t_token_type type)
{
	if (type == TOK_REDIR_IN || type == TOK_REDIR_OUT ||
		type == TOK_APPEND || type == TOK_HEREDOC)
		return (1);
	return (0);
}

int syntax_validation (t_tokens *my_tokens)
{
	t_tokens	*token;

	token = my_tokens;
	while (token)
	{
		if (token -> type == TOK_PIPE)
		{
			if (!token->prev || !token->next)
				return (0);
			if (token->prev->type != TOK_WORD ||
				token->next->type == TOK_PIPE)
				return (0);
		}
		if (is_redirection(token->type))
		{
			if (is_redirection(token->next->type) ||
				token->next->type == TOK_PIPE || !token->next)
				return (0);
		}
		token = token -> next;
	}
	return (1);
}

int main()
{
	t_tokens *my_tokens = NULL;
	//char *input = "<< \'l\'s -la | < grep a >> \"\'$HOME\'\" infile.txt echo \"abdallah\" -n";
	char *test = "> infile ls | > infle";
	token_analyser(test, &my_tokens);
	printf("%d", syntax_valid(my_tokens));
	print_tokens(my_tokens);
	free_tokens(my_tokens);
	return (0);
}

