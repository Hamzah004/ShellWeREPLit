/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 09:41:13 by amufleh           #+#    #+#             */
/*   Updated: 2026/02/18 16:26:02 by amufleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int main()
{
	t_tokens *my_tokens = NULL;
	t_commands *my_commands;
	char *input = "< infile1.txt < infile2.txt \'l\'s -l -a | grep a >> outfile.txt | echo \"\'$HOME\'\" > infile.txt | echo \"abdallah\" -n";
	token_analyser(input, &my_tokens);
	print_tokens(my_tokens);
	printf("----------------------------------\n");
	//printf("%d",count_words(my_tokens));
	my_commands = new_command(my_tokens);
	if (the_parser(my_tokens, my_commands))
		print_commands(my_commands);
	//free_tokens(my_tokens);
	return (0);
}
