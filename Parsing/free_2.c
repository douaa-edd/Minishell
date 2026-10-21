/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:32:08 by schernan          #+#    #+#             */
/*   Updated: 2025/07/26 00:32:09 by schernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_tokens(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head->next;
		free(head->value);
		free(head);
		head = tmp;
	}
}

void	free_cmd(t_cmd *cmd)
{
	t_red	*tmp;
	t_red	*red;

	if (!cmd)
		return ;
	if (cmd->arg)
		free_split(cmd->arg);
	red = cmd->red;
	while (red)
	{
		tmp = red->next;
		free(red->file);
		free(red);
		red = tmp;
	}
	free(cmd);
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		if (env->key)
			free(env->key);
		if (env->value)
			free(env->value);
		free(env);
		env = tmp;
	}
}

void	free_env_array(char **envp, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

void	free_envp(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
		free(envp[i++]);
	free(envp);
	printf("FREE envp\n");
}
