/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddaidi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:19:23 by deddaidi          #+#    #+#             */
/*   Updated: 2025/07/26 00:19:28 by deddaidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*case_of_double_dollar(int *i, char *result)
{
	pid_t	pid;
	char	*pid_str;
	char	*tmp;

	pid = getpid();
	pid_str = ft_itoa(pid);
	if (!pid_str)
		return (result);
	tmp = ft_strjoin(result, pid_str);
	free(result);
	free(pid_str);
	(*i) += 2;
	return (tmp);
}

char	*process_of_expanding(char *word, int *i, char *result, t_env *env)
{
	if (word[*i] == '\'')
		result = case_of_squote(word, i, result);
	else if (word[*i] == '"')
		result = case_of_dquote(word, i, result, env);
	else if (word[*i] == '$' && word[*i + 1] && (ft_isalpha(word[*i + 1])
			|| word[*i + 1] == '_'))
		result = case_of_normal_var(word, i, result, env);
	else if (word[*i] == '$' && word[*i + 1] == '$')
		result = case_of_double_dollar(i, result);
	else if (word[*i] == '$' && word[*i + 1] == '\'')
		result = case_of_var_with_next_char_quote(word, i, result, '\'');
	else if (word[*i] == '$' && word[*i + 1] == '"')
		result = case_of_var_with_next_char_quote(word, i, result, '"');
	else if (word[*i] == '$' && ft_isdigit(word[*i + 1]))
		result = case_of_var_with_next_char_digit(word, i, result);
	else if (word[*i] == '$' && word[*i + 1] == '?')
		result = case_of_var_with_exit_status(i, result);
	else
		result = case_of_word(word, i, result);
	return (result);
}

char	*expand_variable_value(char *word, t_env *env)
{
	int		i;
	char	*result;

	if (!word)
		return (NULL);
	i = 0;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (word[i] && result)
		result = process_of_expanding(word, &i, result, env);
	return (result);
}

int	expand_current_token(t_token **tokens_ptr, t_token *current, t_token *prev,
		t_env *env)
{
	char	*expanded_value;
	t_token	*next;

	next = current->next;
	expanded_value = expand_variable_value(current->value, env);
	if (!expanded_value)
		return (0);
	free(current->value);
	current->value = expanded_value;
	(void)tokens_ptr;
	(void)next;
	(void)prev;
	return (0);
}

void	expand_tokens(t_token **tokens_ptr, t_env *env)
{
	t_token	*current;
	t_token	*prev;
	t_token	*next;

	if (!tokens_ptr)
		return ;
	current = *tokens_ptr;
	prev = NULL;
	while (current)
	{
		next = current->next;
		if (current->type == WORD && !(prev && prev->type == HEREDOC))
		{
			if (expand_current_token(tokens_ptr, current, prev, env))
			{
				current = next;
				continue ;
			}
		}
		prev = current;
		current = next;
	}
}
