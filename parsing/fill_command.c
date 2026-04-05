/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 09:41:13 by amufleh           #+#    #+#             */
/*   Updated: 2026/04/05 13:30:30 by amufleh          ###   ########.fr       */
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

t_commands	*fill_command_struct(char *input, char **env)
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
		return (free_parser(my_commands, my_tokens));
	print_tokens(my_tokens);
	if (!the_parser(my_tokens, my_commands))
		return (free_parser(my_commands, my_tokens));
	if (!yokotenkai(my_commands, env))
		return (free_parser(my_commands, my_tokens));
	remove_empty_arg(my_commands);
	free_tokens(my_tokens);
	return (my_commands);
}

// #include <readline/readline.h>
// int	main(int argc, char **argv, char **env)
// {
// 	t_commands	*my_commands;
// 	//char *input = "echo \"$HOME\" | grep a > \'file2\' | echo \"abdallah\" | echo \'\"abdallah\"\' | echo \"$HOME\" | echo \"\'$HOME\'\" | echo \' \'$HOME\' \'";
// 	// char *input = "echo \"abdallah\"";
// 	char *input = ">test" ;
// 	my_commands =  fill_command_struct(input, env);
// 	if (my_commands)
// 	{
// 		print_commands(my_commands);
// 		free_commands(my_commands);
// 	}
// 	//printf("\n->%s", polish("       Abdallah        Almufleh     "));
// 	return (0);
// }
