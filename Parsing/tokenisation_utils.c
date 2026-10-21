/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddaidi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:20:40 by deddaidi          #+#    #+#             */
/*   Updated: 2025/07/26 00:20:42 by deddaidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

char	*get_operator(char *input, int *i, t_quote *quote_type)
{
	char	*operator;

	if (!ft_strncmp(input + *i, ">>", 2) || !ft_strncmp(input + *i, "<<", 2))
	{
		operator = ft_substr(input, *i, 2);
		if (!operator)
			return (NULL);
		*i += 2;
	}
	else
	{
		operator = ft_substr(input, *i, 1);
		if (!operator)
			return (NULL);
		(*i)++;
	}
	*quote_type = NONE;
	return (operator);
}

int	handle_quotes(char *input, int *i, t_quote *quote_type)
{
	char	quote;

	if (!input || !input[*i])
		return (0);
	quote = input[*i];
	if (*quote_type == NONE)
	{
		if (quote == '\'')
			*quote_type = SINGLE;
		else if (quote == '"')
			*quote_type = DOUBLE;
		else
			return (0);
	}
	(*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] == quote)
		(*i)++;
	else
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
		return (0);
	}
	return (1);
}

char	*get_word(char *input, int *i, t_quote *quote_type)
{
	int		start;
	char	*res;
	int		pos;

	start = *i;
	*quote_type = NONE;
	while (input[*i] && !is_space(input[*i]) && input[*i] != '|'
		&& input[*i] != '>' && input[*i] != '<')
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			if (!handle_quotes(input, i, quote_type))
				return (NULL);
		}
		else
			(*i)++;
	}
	pos = *i - start;
	res = ft_substr(input, start, pos);
	return (res);
}

char	*process_input(char *input, int *i, t_quote *quote_type)
{
	char	*value;

	while (input[*i] && is_space(input[*i]))
		(*i)++;
	if (!input[*i])
		return (NULL);
	if (input[*i] == '|' || input[*i] == '>' || input[*i] == '<')
		value = get_operator(input, i, quote_type);
	else
		value = get_word(input, i, quote_type);
	return (value);
}
