/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddaidi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:21:27 by deddaidi          #+#    #+#             */
/*   Updated: 2025/07/26 00:21:29 by deddaidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	env_count(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (env->key && env->value && env->value[0] != '\0')
			count++;
		env = env->next;
	}
	return (count);
}

char	**upd_env(t_env *env)
{
	int		i;
	char	**envp;

	envp = malloc(sizeof(char *) * (env_count(env) + 1));
	if (!envp)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->key && env->value && env->value[0] != '\0')
		{
			envp[i] = ft_strjoin_three(env->key, "=", env->value);
			if (!envp[i])
				return (free_env_array(envp, i), NULL);
			i++;
		}
		env = env->next;
	}
	envp[i] = NULL;
	return (envp);
}

int	handle_env_error(char *arg)
{
	if (is_directory(arg))
	{
		ft_putstr_fd("env: ‘", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("’: Permission denied\n", 2);
		return (126);
	}
	ft_putstr_fd("env: ‘", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("’: No such file or directory\n", 2);
	return (127);
}

int	ft_env(t_cmd *cmd, t_env *env)
{
	char	**env_array;
	int		i;

	if (cmd->arg[1])
		return (handle_env_error(cmd->arg[1]));
	env_array = upd_env(env);
	if (!env_array)
		return (1);
	i = 0;
	while (env_array[i])
	{
		printf("%s\n", env_array[i]);
		free(env_array[i++]);
	}
	free(env_array);
	return (0);
}
