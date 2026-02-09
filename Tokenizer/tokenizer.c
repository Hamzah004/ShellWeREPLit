#include <unistd.h>
#include <stdio.h>
#include "./libft/libft.h"

typedef enum
{
	TOK_WORD,
	TOK_PIPE,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_APPEND,
	TOK_HEREDOC,
	TOK_SINGLE_QUOTE,
	TOK_DOUBLE_QUOTE
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_tokens;

void free_tokens(t_tokens *head)
{
	t_tokens *tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->value);
		free(tmp);
	}
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (1);
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char *remove_spaces(char *str)
{
	int start;
	int end;
	int i;
	char *new;

	start = 0;
	i = 0;
	if (!str)
		return (NULL);
	while (str[start] == ' ')
		start++;
	if (str[start] == '\0')
		return ft_strdup("");
	end = ft_strlen(str) - 1;
	while (end > start && str[end] == ' ')
		end--;
	new = malloc((end - start) + 2);
	if (!new)
		return (NULL);
	while (i < (end - start) + 1)
	{
		new[i] = str[start + i];
		i++;
	}
	new[i] = '\0';
	return (new);
}


int	special_character(char character)
{
	return (character == '|' || character == '<' || character == '>'
		|| character == '\'' || character == '\"' || character == '-');
}

t_tokens *new_token(t_token_type type, char *value)
{
	t_tokens *token;

	token = malloc(sizeof(t_tokens));
	if (!token)
		return NULL;
	token->type = type;
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return NULL;
	}
	token->next = NULL;
	return (token);
}

t_token_type set_token_type(char *str)
{
	if (!str)
		return (TOK_WORD);
	if (ft_strcmp(str, "|") == 0)
		return (TOK_PIPE);
	if (ft_strcmp(str, "<") == 0)
		return (TOK_REDIR_IN);
	if (ft_strcmp(str, ">") == 0)
		return (TOK_REDIR_OUT);
	if (ft_strcmp(str, ">>") == 0)
	return (TOK_APPEND);
	if (ft_strcmp(str, "<<") == 0)
		return (TOK_HEREDOC);
	if (ft_strcmp(str, "\"") == 0)
		return (TOK_DOUBLE_QUOTE);
	if (ft_strcmp(str, "\'") == 0)
		return 	(TOK_SINGLE_QUOTE);
	return TOK_WORD;
}

void	add_token(char *str, t_tokens **tokens)
{
	t_tokens		*new;
	t_tokens		*tmp;
	char			*str_no_space;
	t_token_type	type;

	if (!str || ft_strcmp(str, " ") == 0 || !str[0])
		return ;
	str_no_space = remove_spaces(str);
	type = set_token_type(str_no_space);
	new = new_token(type, str_no_space);
	if (!*tokens)
	{
		// TODO free the things
		*tokens = new;
		return ;
	}
	tmp = *tokens;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

char	*free_realloc(char *token)
{
	free(token);
	token = calloc(1, sizeof(char));
	if (!token)
	{
		//TODO add fun to free
		return (NULL);
	}
	return (token);
}

char	*append_char (char *str, char c)
{
	int		size;
	int		i;
	char	*tmp;

	i = 0;
	size = ft_strlen(str);
	tmp = realloc(str, size + 2);
	while (str[i])
	{
		tmp[i] = str[i];
		i++;
	}
	tmp[i] = c;
	tmp[i + 1] = '\0';
	return (tmp);
}

void	token_analyser(char *command, t_tokens *my_tokens)
{
	int			i;
	char		*token;

	i = 0;
	token = calloc(1, sizeof(char));
	while (command[i])
	{
		if (!special_character(command[i]))
			token = append_char (token, command[i]);
		else
		{
			add_token(token, &my_tokens);
			token = free_realloc(token);
			token = append_char (token, command[i]);
			add_token(token, &my_tokens);
			if (command[i] != '-')
				token = free_realloc(token);
		}
		i++;
	}
	add_token(token, &my_tokens);
}

t_tokens	*init_tokens(void)
{
	t_tokens *token;

	token = malloc(sizeof(t_tokens));
	if (!token)
		return NULL;
	token->type = TOK_WORD;
	token->value = NULL;
	token->next = NULL;
	return (token);
}

void print_tokens(t_tokens *head)
{
	t_tokens *tmp = head;

	while (tmp)
	{
		printf("Token: \"%s\"  |  Type: ", tmp->value);
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
		else if (tmp->type == TOK_SINGLE_QUOTE)
			printf("SINGLE QUOTE");
		else if (tmp->type == TOK_DOUBLE_QUOTE)
			printf("DOUBLE QUOTE");
		else
			printf("UNKNOWN");
		printf("\n");
		tmp = tmp->next;
	}
}

int	main()
{
	t_tokens	*my_tokens;

	char *input = "ls -la | < grep a >> \"$HOME\" infile.txt";

	my_tokens = init_tokens();
	token_analyser(input, my_tokens);
	print_tokens(my_tokens);
}
