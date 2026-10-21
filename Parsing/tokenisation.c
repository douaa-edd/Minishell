/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:33:14 by schernan          #+#    #+#             */
/*   Updated: 2025/07/26 00:33:16 by schernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*new_token(char *value, t_token_type type, t_quote quote_type)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
	{
		free(value);
		return (NULL);
	}
	new_token->value = value;
	new_token->type = type;
	new_token->quote_type = quote_type;
	new_token->next = NULL;
	return (new_token);
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*tmp;

	if (!new_token)
		return ;
	if (!*head)
		*head = new_token;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_token;
	}
}

t_token_type	get_token_type(char *value)
{
	if (!value)
		return (WORD);
	if (!ft_strncmp(value, ">>", 2))
		return (APPEND);
	else if (!ft_strncmp(value, "<<", 2))
		return (HEREDOC);
	else if (!ft_strncmp(value, "|", 1))
		return (PIPE);
	else if (!ft_strncmp(value, ">", 1))
		return (OUTPUT);
	else if (!ft_strncmp(value, "<", 1))
		return (INPUT);
	return (WORD);
}

int	handle_token(char *input, int *i, t_quote *quote_type, t_token **tokens)
{
	char			*value;
	t_token			*new_tok;
	t_token_type	type;

	value = process_input(input, i, quote_type);
	if (!value)
		return (1);
	if (!value[0])
	{
		free(value);
		*quote_type = NONE;
		return (1);
	}
	type = get_token_type(value);
	new_tok = new_token(value, type, *quote_type);
	if (!new_tok)
	{
		free(value);
		return (0);
	}
	add_token(tokens, new_tok);
	*quote_type = NONE;
	return (1);
}

t_token	*tokenize_line(char *input)
{
	t_token	*tokens;
	int		i;
	t_quote	quote_type;

	if (!input)
		return (NULL);
	tokens = NULL;
	i = 0;
	quote_type = NONE;
	while (input[i])
	{
		if (!handle_token(input, &i, &quote_type, &tokens))
		{
			free_tokens(tokens);
			return (NULL);
		}
	}
	return (tokens);
}
