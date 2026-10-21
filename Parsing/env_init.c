/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddaidi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:19:08 by deddaidi          #+#    #+#             */
/*   Updated: 2025/07/26 00:19:10 by deddaidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_split(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

t_env	*new_env_node(const char *key, const char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = ft_strdup("");
	if (!node->key || !node->value)
	{
		free(node->key);
		free(node->value);
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

void	add_env_node(t_env **env_list, t_env *new_node)
{
	t_env	*cur;

	if (!new_node)
		return ;
	if (!*env_list)
		*env_list = new_node;
	else
	{
		cur = *env_list;
		while (cur->next)
			cur = cur->next;
		cur->next = new_node;
	}
}

void	free_env_list(t_env *env_list)
{
	t_env	*tmp;

	while (env_list)
	{
		tmp = env_list->next;
		free(env_list->key);
		free(env_list->value);
		free(env_list);
		env_list = tmp;
	}
}

t_env	*init_env(char **envp)
{
	t_env	*env_list;
	t_env	*node;
	char	**split;
	int		i;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		split = ft_split(envp[i], '=');
		if (split && split[0])
		{
			if (split[1])
				node = new_env_node(split[0], split[1]);
			else
				node = new_env_node(split[0], "");
			if (node)
				add_env_node(&env_list, node);
		}
		free_split(split);
		i++;
	}
	return (env_list);
}
