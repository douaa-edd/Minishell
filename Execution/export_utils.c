/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddaidi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:21:39 by deddaidi          #+#    #+#             */
/*   Updated: 2025/07/26 00:21:42 by deddaidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*create_export_line(t_env *env)
{
	char	*tmp;
	char	*tmp2;
	char	*result;

	if (!env || !env->key)
		return (NULL);
	if (env->value && env->value[0] != '\0')
	{
		tmp = ft_strjoin(env->value, "\"");
		if (!tmp)
			return (NULL);
		tmp2 = ft_strjoin("=\"", tmp);
		free(tmp);
		if (!tmp2)
			return (NULL);
		result = ft_strjoin(env->key, tmp2);
		free(tmp2);
	}
	else
		result = ft_strdup(env->key);
	return (result);
}

char	**free_export_array(char **arr, int i)
{
	while (--i >= 0)
		free(arr[i]);
	free(arr);
	return (NULL);
}

char	**env_to_export_array(t_env *env)
{
	int		i;
	int		size;
	char	**arr;

	i = 0;
	size = count_env_vars(env);
	arr = malloc(sizeof(char *) * (size + 1));
	if (!arr)
		return (NULL);
	while (env)
	{
		if (env->key && ft_strcmp(env->key, "_") == 0)
		{
			env = env->next;
			continue ;
		}
		arr[i] = create_export_line(env);
		if (!arr[i])
			return (free_export_array(arr, i));
		i++;
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}

void	update_existing_env(t_env *cur, char *value, int is_append)
{
	char	*new_value;

	new_value = NULL;
	if (is_append && value)
	{
		if (cur->value)
			new_value = ft_strjoin(cur->value, value);
		else
			new_value = ft_strdup(value);
		free(cur->value);
		cur->value = new_value;
	}
	else
	{
		free(cur->value);
		if (value)
			cur->value = ft_strdup(value);
		else
			cur->value = NULL;
	}
}

void	handle_env_update(t_env **env_list, t_env *cur, void **all,
		int is_append)
{
	char	*key;
	char	*value;

	key = (char *)all[0];
	value = (char *)all[1];
	while (cur)
	{
		if (cur->key && ft_strcmp(cur->key, key) == 0)
		{
			update_existing_env(cur, value, is_append);
			free(key);
			if (value)
				free(value);
			return ;
		}
		cur = cur->next;
	}
	add_env_node(env_list, new_env_node(key, value));
	free(key);
	if (value)
		free(value);
}
