<<<<<<< Updated upstream
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: amufleh <amufleh@student.42.fr>            +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2026/02/11 16:20:29 by amufleh           #+#    #+#             */
/*   Updated: 2026/02/11 16:20:29 by amufleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

void	free_tokens(t_tokens *head)
{
	t_tokens *tmp;

	while (head)
	{
		tmp = head->next;
		free(head->value);
		free(head);
		head = tmp;
	}
}

int	add_token(t_tokens **tokens, char *value)
{
	t_tokens		*new;
	t_tokens		*tmp;
	t_token_type	type;

	type = set_token_type(value);
	new = new_token(value, type);
	if (!new)
		return (0);
	if (!*tokens)
		*tokens = new;
	else
	{
		tmp = *tokens;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (1);
}

int	extract_tokin(t_tokens **my_tokens, char *command, int start, int end)
{
	char	*token;

	token = ft_substr(command, start, end);
	if (!token)
		return (0);
	if (!add_token(my_tokens, token))
		return (0);
	return (1);
}

int	handel_operators(t_tokens **my_tokens, char *command, int *i)
{
	if ((command[*i] == '<' && command[*i + 1] == '<')
	|| (command[*i] == '>' && command[*i + 1] == '>'))
	{
		if (!extract_tokin(my_tokens, command, *i, 2))
			return (0);
		*i += 2;
	}
	else
	{
		if (!extract_tokin(my_tokens, command, *i, 1))
			return (0);
		*i += 1;
	}
	return (1);
}

int	handle_word(t_tokens **my_tokens, char *command, int *i)
{
	int	start;

	start = *i;
	while (command[*i] && !is_separator(command[*i]))
	{
		if (command[*i] == '\'' || command[*i] == '\"')
		{
			char quote = command[(*i)++];
			while (command[*i] && command[*i] != quote)
				(*i)++;
		}
		if (command[*i])
			(*i)++;
	}
	if (!extract_tokin(my_tokens, command, start, *i - start))
		return (0);
	return (1);
}

int	token_analyser(char *command, t_tokens **my_tokens)
{
	int		i;
	int		start;
	char	*token;
	char	quote;

	i = 0;
	if (!command)
		return (0);
	while (command[i])
	{
		if (command[i] == ' ' || command[i] == '\t')
		{
			i++;
			continue ;
		}
		if (is_operator(command[i]))
			if (!handel_operators(my_tokens, command, &i))
				return (0);
		else
			if (!handle_word(my_tokens, command, &i))
				return (0);
	}
	return (1);
}

void	print_tokens(t_tokens *head)
{
	t_tokens *tmp = head;
 	while (tmp)
	{
		printf("Token: [%s]  ", tmp->value);
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
		else
			printf("UNKNOWN");
		printf("\n");
		tmp = tmp->next;
	}
}

int	main(void)
{
	t_tokens *my_tokens = NULL;

	char *input = "<< \'l\'s
		-la | < grep a >> \"\'$HOME\'\" infile.txt echo \"abdallah\" -n";
	token_analyser(input, &my_tokens);
	print_tokens(my_tokens);
	return (0);
}
=======
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbani-at <hbani-at@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 16:37:33 by hbani-at          #+#    #+#             */
/*   Updated: 2026/02/11 16:37:37 by hbani-at         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include <stdio.h>
#include <unistd.h>
#include "../include/minishell.h"

void	free_tokens(t_tokens *head)
{
	t_tokens	*tmp;

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

char	*remove_spaces(char *str)
{
	int		start;
	int		end;
	int		i;
	char	*new;

	start = 0;
	i = 0;
	if (!str)
		return (NULL);
	while (str[start] == ' ')
		start++;
	if (str[start] == '\0')
		return (ft_strdup(""));
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

t_tokens	*new_token(t_token_type type, char *value)
{
	t_tokens	*token;

	token = malloc(sizeof(t_tokens));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->next = NULL;
	return (token);
}

t_token_type	set_token_type(char *str)
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
		return (TOK_SINGLE_QUOTE);
	return (TOK_WORD);
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
		// TODO add fun to free
		return (NULL);
	}
	return (token);
}

char	*append_char(char *str, char c)
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
	int		i;
	char	*token;

	i = 0;
	token = calloc(1, sizeof(char));
	while (command[i])
	{
		if (!special_character(command[i]))
			token = append_char(token, command[i]);
		else
		{
			add_token(token, &my_tokens);
			token = free_realloc(token);
			token = append_char(token, command[i]);
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
	t_tokens	*token;

	token = malloc(sizeof(t_tokens));
	if (!token)
		return (NULL);
	token->type = TOK_WORD;
	token->value = NULL;
	token->next = NULL;
	return (token);
}

void	print_tokens(t_tokens *head)
{
	t_tokens	*tmp;

	tmp = head;
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

int	main(void)
{
	t_tokens	*my_tokens;
	char		*input;

	input = "ls -la | < grep a >> \"$HOME\" infile.txt";
	my_tokens = init_tokens();
	token_analyser(input, my_tokens);
	print_tokens(my_tokens);
}
>>>>>>> Stashed changes
