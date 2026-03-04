/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 09:41:13 by amufleh           #+#    #+#             */
/*   Updated: 2026/03/04 13:55:48 by amufleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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

int	fill_command_struct(char *input)
{
	t_tokens	*my_tokens;
	t_commands	*my_commands;

	if (!input_validation(input))
		return (0);
	my_tokens = NULL;
	my_commands = new_command();
	if (!my_commands)
		return (0);
	token_analyser(input, &my_tokens);
	if (!syntax_validation(my_tokens))
		return (free_parser(my_commands, my_tokens, 1));
	print_tokens(my_tokens);
	if (the_parser(my_tokens, my_commands))
		print_commands(my_commands);
	free_parser(my_commands, my_tokens, 1);
	return (1);
}
 #include <readline/readline.h>

 int main()
{
	char *input = " ";

	fill_command_struct(input);
	return (0);
}
