/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:35:59 by schernan          #+#    #+#             */
/*   Updated: 2025/07/26 00:36:01 by schernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_shlvl_value(t_env *env, char *new_val)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, "SHLVL") == 0)
		{
			free(tmp->value);
			tmp->value = new_val;
			break ;
		}
		tmp = tmp->next;
	}
}

void	add_shlvl(t_env **env)
{
	t_env	*node;

	node = new_env_node("SHLVL", "1");
	add_env_node(env, node);
}

void	increment_shlvl(t_env **env)
{
	char	*shlvl_str;
	char	*new_val;
	int		shlvl;

	shlvl_str = get_env_value(*env, "SHLVL");
	if (!shlvl_str)
	{
		add_shlvl(env);
		return ;
	}
	shlvl = ft_atoi(shlvl_str);
	shlvl++;
	if (shlvl >= 1000)
	{
		ft_putstr_fd("minishell: warning: shell level (", STDERR_FILENO);
		ft_putnbr_fd(shlvl, STDERR_FILENO);
		ft_putstr_fd(") too high, resetting to 1\n", STDERR_FILENO);
		shlvl = 1;
	}
	else if (shlvl < 0)
		shlvl = 0;
	new_val = ft_itoa(shlvl);
	if (!new_val)
		return ;
	set_shlvl_value(*env, new_val);
}
