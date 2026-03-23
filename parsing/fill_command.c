/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 18:15:00 by amufleh           #+#    #+#             */
/*   Updated: 2026/03/15 18:15:00 by amufleh          ###   ########.fr       */
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
	while (input[i])
	{
		c = input[i];
		if ((c == '|' || c == '>' || c == '<') && (i == 0 || input[i
				- 1] == ' '))
			return (0);
		if (c == '\'' || c == '\"')
			return (1);
		if (c != ' ')
			return (1);
		i++;
	}
	return (0);
}

t_commands	*fill_command_struct(char *input)
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
	{
		free_tokens(my_tokens);
		// free_parser(my_commands);
		return (NULL);
	}
	print_tokens(my_tokens);
	if (!the_parser(my_tokens, my_commands))
		return (NULL);
	// command_expansion(my_commands);
	free_tokens(my_tokens);
	return (my_commands);
}
