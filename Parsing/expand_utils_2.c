/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:31:50 by schernan          #+#    #+#             */
/*   Updated: 2025/07/26 00:31:52 by schernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strjoin_char(char *str, char c)
{
	char	*new_str;
	int		i;

	if (!str)
		str = ft_strdup("");
	if (!str)
		return (NULL);
	new_str = malloc(sizeof(char) * (ft_strlen(str) + 2));
	if (!new_str)
	{
		free(str);
		return (NULL);
	}
	i = 0;
	while (str[i])
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i++] = c;
	new_str[i] = '\0';
	free(str);
	return (new_str);
}

char	*get_env_value(t_env *env, char *key)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

char	*append_value(char *result, const char *value)
{
	int	j;

	j = 0;
	while (value[j])
	{
		result = ft_strjoin_char(result, value[j++]);
		if (!result)
			return (NULL);
	}
	return (result);
}

char	*case_of_normal_var(char *word, int *i, char *result, t_env *env)
{
	int		start;
	char	*key;
	char	*value;

	start = ++(*i);
	while (word[*i] && (ft_isalnum(word[*i]) || word[*i] == '_'))
		(*i)++;
	key = ft_substr(word, start, *i - start);
	if (!key)
		return (result);
	if (ft_strcmp(key, "_") == 0)
	{
		value = get_env_value(env, "_");
		free(key);
		if (!value)
			value = "";
		return (append_value(result, value));
	}
	value = get_env_value(env, key);
	free(key);
	if (!value)
		value = "";
	return (append_value(result, value));
}

char	*case_of_var_with_exit_status(int *i, char *result)
{
	char	*tmp;
	char	*joined;

	tmp = ft_itoa(g_global.exit_status);
	if (!tmp)
		return (result);
	joined = ft_strjoin(result, tmp);
	free(result);
	free(tmp);
	(*i) += 2;
	return (joined);
}
