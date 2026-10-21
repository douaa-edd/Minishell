/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:33:37 by schernan          #+#    #+#             */
/*   Updated: 2025/07/26 00:33:39 by schernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cd_error(int code)
{
	if (code == 0)
		write(2, "cd: too many arguments\n", 23);
	else if (code == 1)
		write(2, "cd: HOME not set\n", 18);
	else if (code == 2)
		perror("cd");
	else if (code == 3)
		write(2, "cd: OLDPWD not set\n", 20);
	g_global.exit_status = 1;
}

t_env	*get_env_node(t_env *env_list, const char *key)
{
	while (env_list)
	{
		if (env_list->key && ft_strcmp(env_list->key, key) == 0)
			return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}

void	update_oldpwd(t_env **env_list, const char *old_pwd)
{
	t_env	*oldpwd_node;

	oldpwd_node = get_env_node(*env_list, "OLDPWD");
	if (oldpwd_node)
	{
		free(oldpwd_node->value);
		oldpwd_node->value = ft_strdup(old_pwd);
	}
	else
		add_env_node(env_list, new_env_node("OLDPWD", old_pwd));
}
