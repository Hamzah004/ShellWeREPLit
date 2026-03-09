#include "../include/parsing.h"

int main()
{
	t_tokens *my_tokens;
	t_cmd *my_commands;
	char *test_cases[] = {
		"echo hello",
		"echo \"hello world\"",
		"echo 'single quoted'",
		"ls -la | grep test",
		"cat < input.txt",
		"echo test > output.txt",
		"cat << EOF",
		"ls >> results.txt",
		NULL
	};

	int i = 0;
	while (test_cases[i])
	{
		printf("\n===== Test: %s =====\n", test_cases[i]);
		my_tokens = NULL;
		
		if (!token_analyser(test_cases[i], &my_tokens))
		{
			printf("Tokenizer failed\n");
			i++;
			continue;
		}
		
		print_tokens(my_tokens);
		printf("----------------------------------\n");
		
		my_commands = new_command(my_tokens);
		if (!my_commands)
		{
			printf("Failed to create command\n");
			free_tokens(my_tokens);
			i++;
			continue;
		}
		
		if (the_parser(my_tokens, my_commands))
		{
			print_commands(my_commands);
			free_commands(my_commands);
		}
		else
		{
			printf("Parser validation failed\n");
			free_commands(my_commands);
		}
		
		free_tokens(my_tokens);
		i++;
	}

	return (0);
}
