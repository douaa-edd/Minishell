/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:36:21 by schernan          #+#    #+#             */
/*   Updated: 2025/07/26 00:36:23 by schernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	remove_env_var(t_env **env_list, const char *key_to_remove)
{
	t_env	*curr;
	t_env	*prev;

	if (!env_list || !key_to_remove)
		return ;
	curr = *env_list;
	prev = NULL;
	while (curr)
	{
		if (curr->key && ft_strcmp(curr->key, key_to_remove) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*env_list = curr->next;
			free(curr->key);
			free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

int	ft_unset(char **args, t_env **env_list)
{
	int	i;

	if (!args || !env_list)
		return (1);
	i = 1;
	while (args[i])
	{
		remove_env_var(env_list, args[i]);
		i++;
	}
	return (0);
}
