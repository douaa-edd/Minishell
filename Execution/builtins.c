/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddaidi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:21:03 by deddaidi          #+#    #+#             */
/*   Updated: 2025/07/26 00:21:05 by deddaidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "echo") || !ft_strcmp(cmd,
			"pwd") || !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env"));
}

int	exec_builtin(t_cmd *mochkil, t_cmd *cmd, t_env **env, t_heredoc_file *files)
{
	void	*all[2];

	if (!cmd || !cmd->arg || !cmd->arg[0])
		return (0);
	if (!ft_strcmp(cmd->arg[0], "cd"))
		g_global.exit_status = ft_cd(cmd->arg, env);
	else if (!ft_strcmp(cmd->arg[0], "echo"))
		g_global.exit_status = ft_echo(cmd->arg);
	else if (!ft_strcmp(cmd->arg[0], "pwd"))
		g_global.exit_status = ft_pwd(*env);
	else if (!ft_strcmp(cmd->arg[0], "exit"))
	{
		all[0] = mochkil;
		all[1] = files;
		g_global.exit_status = ft_exit(cmd->arg, false, all, *env);
	}
	else if (!ft_strcmp(cmd->arg[0], "export"))
		g_global.exit_status = ft_export(cmd->arg, env);
	else if (!ft_strcmp(cmd->arg[0], "unset"))
		g_global.exit_status = ft_unset(cmd->arg, env);
	else if (!ft_strcmp(cmd->arg[0], "env"))
		g_global.exit_status = ft_env(cmd, *env);
	else
		return (0);
	return (1);
}
