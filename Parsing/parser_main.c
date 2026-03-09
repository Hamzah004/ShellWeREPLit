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

#include "../include/parsing.h"
#include <string.h>

int main()
{
	t_tokens *my_tokens;
	t_cmd *my_commands;
	char buffer[1024];

	printf("Parser Test - Enter commands (or 'exit' to quit)\n");
	printf("Example: echo hello | grep e > output.txt\n");
	printf("========================================\n\n");

	while (1)
	{
		printf("$ ");
		if (!fgets(buffer, sizeof(buffer), stdin))
			break;
		
		// Remove newline from input
		int len = strlen(buffer);
		if (len > 0 && buffer[len - 1] == '\n')
			buffer[len - 1] = '\0';
		
		// Check for exit command
		if (strcmp(buffer, "exit") == 0)
			break;
		
		// Skip empty input
		if (strlen(buffer) == 0)
			continue;
		
		my_tokens = NULL;
		
		// Tokenize input
		if (!token_analyser(buffer, &my_tokens))
		{
			printf("Error: Tokenizer failed\n\n");
			continue;
		}
		
		printf("\nTokens:\n");
		print_tokens(my_tokens);
		printf("----------------------------------\n");
		
		// Create command structure
		my_commands = new_command(my_tokens);
		if (!my_commands)
		{
			printf("Error: Failed to create command\n");
			free_tokens(my_tokens);
			printf("\n");
			continue;
		}
		
		// Parse tokens into commands
		if (the_parser(my_tokens, my_commands))
		{
			printf("\nParsed Commands:\n");
			print_commands(my_commands);
			free_commands(my_commands);
		}
		else
		{
			printf("Error: Syntax validation failed\n");
			free_commands(my_commands);
		}
		
		free_tokens(my_tokens);
		printf("\n");
	}

	printf("\nExiting parser test...\n");
	return (0);
}
