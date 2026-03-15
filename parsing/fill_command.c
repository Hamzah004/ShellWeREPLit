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
		if ((c == '|' || c == '>' || c == '<') && (i == 0
				|| input[i - 1] == ' '))
			return (0);
		if (c == '\'' || c == '\"')
			return (1);
		if (c != ' ')
			return (1);
		i++;
	}
	return (0);
}

int	fill_command_struct(char *input, t_tokens *my_token, t_commands *my_command)
{
	if (!input_validation(input))
		return (0);
	my_token = NULL;
	my_command = new_command();
	if (!my_command)
		return (0);
	token_analyser(input, &my_token);
	if (!syntax_validation(my_token))
		return (free_parser(my_command, my_token, 1));
	// print_tokens(my_token);
	if (the_parser(my_token, my_command))
		print_commands(my_command);
	free_parser(my_command, my_token, 1);
	return (1);
}
