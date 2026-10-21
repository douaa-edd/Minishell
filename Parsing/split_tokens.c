/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddaidi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:20:20 by deddaidi          #+#    #+#             */
/*   Updated: 2025/07/26 00:20:27 by deddaidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*create_split_tokens(char **parts)
{
	t_token	*first_new;
	t_token	*last_new;
	t_token	*new_tok;
	int		i;

	first_new = NULL;
	last_new = NULL;
	i = 0;
	while (parts[i])
	{
		new_tok = new_token(ft_strdup(parts[i]), WORD, NONE);
		if (!new_tok)
			return (NULL);
		if (!first_new)
			first_new = new_tok;
		else
			last_new->next = new_tok;
		last_new = new_tok;
		i++;
	}
	return (first_new);
}

void	connect_new_tokens(t_token **tokens_ptr, t_token *prev, t_token *curr,
		t_token *first_new)
{
	t_token	*last_new;

	last_new = first_new;
	while (last_new->next)
		last_new = last_new->next;
	last_new->next = curr->next;
	if (prev)
		prev->next = first_new;
	else
		*tokens_ptr = first_new;
	free(curr->value);
	free(curr);
}

int	split_and_replace(t_token **tokens_ptr, t_token *prev, t_token *curr,
		t_token **new_last)
{
	char	**parts;
	t_token	*first_new;

	parts = ft_split(curr->value, ' ');
	if (!parts)
		return (0);
	first_new = create_split_tokens(parts);
	free_split(parts);
	if (!first_new)
		return (0);
	connect_new_tokens(tokens_ptr, prev, curr, first_new);
	*new_last = first_new;
	while ((*new_last)->next)
		*new_last = (*new_last)->next;
	return (1);
}

void	split_token_on_spaces(t_token **tokens_ptr)
{
	t_token	*curr;
	t_token	*prev;
	t_token	*new_last;

	curr = *tokens_ptr;
	prev = NULL;
	while (curr)
	{
		if (curr->type == WORD && curr->quote_type == NONE
			&& ft_strchr(curr->value, ' '))
		{
			if (!split_and_replace(tokens_ptr, prev, curr, &new_last))
				return ;
			curr = new_last;
		}
		prev = curr;
		curr = curr->next;
	}
}
