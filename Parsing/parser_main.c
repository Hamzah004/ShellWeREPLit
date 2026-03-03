/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 09:41:13 by amufleh           #+#    #+#             */
/*   Updated: 2026/03/03 11:41:39 by amufleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	fill_command_struct(char *input)
{
	t_tokens	*my_tokens;
	t_commands	*my_commands;

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

int main()
{
	char *input = "< infile1.txt < infile2.txt \'l\'s -l -a | grep a >> outfile.txt | echo \"\'$HOME\'\" > infile.txt | echo \"abdallah\" -n | cat outfile \" <infile\" ";
	fill_command_struct(input);
	return (0);
}
