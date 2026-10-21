/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:31:38 by schernan          #+#    #+#             */
/*   Updated: 2025/07/26 00:31:41 by schernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*case_of_var_with_next_char_digit(char *word, int *i, char *result)
{
	(*i) += 2;
	while (word[*i])
	{
		result = ft_strjoin_char(result, word[(*i)++]);
		if (!result)
			return (NULL);
	}
	return (result);
}

char	*case_of_var_with_next_char_quote(char *word, int *i, char *result,
		char delimiter)
{
	(*i)++;
	while (word[*i] && word[*i] != delimiter)
	{
		result = ft_strjoin_char(result, word[(*i)++]);
		if (!result)
			return (NULL);
	}
	if (word[*i])
		(*i)++;
	return (result);
}

char	*case_of_word(char *word, int *i, char *result)
{
	result = ft_strjoin_char(result, word[*i]);
	(*i)++;
	return (result);
}

char	*case_of_dquote(char *word, int *i, char *result, t_env *env)
{
	(*i)++;
	while (word[*i] && word[*i] != '"')
	{
		if (word[*i] == '$' && word[*i + 1] && (ft_isalnum(word[*i + 1])
				|| word[*i + 1] == '_'))
			result = case_of_normal_var(word, i, result, env);
		else if (word[*i] == '$' && word[*i + 1] == '?')
			result = case_of_var_with_exit_status(i, result);
		else
		{
			result = ft_strjoin_char(result, word[(*i)++]);
			if (!result)
				return (NULL);
		}
	}
	if (word[*i])
		(*i)++;
	return (result);
}

char	*case_of_squote(char *word, int *i, char *result)
{
	(*i)++;
	while (word[*i] && word[*i] != '\'')
	{
		result = ft_strjoin_char(result, word[(*i)++]);
		if (!result)
			return (NULL);
	}
	if (word[*i])
		(*i)++;
	return (result);
}
