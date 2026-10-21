/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddaidi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:23:27 by deddaidi          #+#    #+#             */
/*   Updated: 2025/07/26 00:23:30 by deddaidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	run_builtin_command(t_cmd *m, t_cmd *cmd, t_env **env,
		t_heredoc_file *files)
{
	pid_t	pid;
	void	*all[2];

	all[0] = m;
	all[1] = files;
	if (ft_strcmp(cmd->arg[0], "exit") == 0)
	{
		ft_exit(cmd->arg, true, all, *env);
		return (1);
	}
	if (cmd->red != NULL)
	{
		pid = fork();
		if (pid == 0)
		{
			g_global.is_child = 1;
			reset_signals();
			if (apply_redirections(cmd->red) < 0)
				cleaner(*env, m, NULL, 1);
			cleaner(*env, m, NULL, exec_builtin(m, cmd, env, files));
		}
		return (wait_and_update_status(pid));
	}
	return (exec_builtin(m, cmd, env, files));
}

void	run_child(t_cmd *mochkil, t_cmd *cmd, t_env **env,
		t_heredoc_file *files)
{
	char	*path;
	char	**envp;
	void	*all[4];

	reset_signals();
	if (apply_redirections(cmd->red) < 0)
	{
		cleaner(*env, cmd, files, 1);
		free_envp(envp);
	}
	all[0] = mochkil;
	all[1] = cmd;
	all[2] = *env;
	all[3] = files;
	path = prepare_exec(cmd, env, &envp, all);
	if (!path)
	{
		cleaner(*env, cmd, NULL, g_global.exit_status);
		free_envp(envp);
	}
	execve(path, cmd->arg, envp);
	perror("execve");
	free(path);
	free_envp(envp);
	cleaner(*env, cmd, NULL, 1);
}

int	exec_external_cmd(t_cmd *mochkil, t_cmd *cmd, t_env **env,
		t_heredoc_file *files)
{
	pid_t	pid;
	int		r;

	g_global.is_child = 1;
	pid = fork();
	if (pid == 0)
	{
		g_global.is_child = 1;
		run_child(mochkil, cmd, env, files);
	}
	r = wait_and_update_status(pid);
	g_global.is_child = 0;
	return (r);
}

void	handle_redirection_only(t_env *env, t_cmd *cmd, t_cmd *mochkil,
		t_heredoc_file *files)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		g_global.is_child = 1;
		reset_signals();
		if (apply_redirections(cmd->red) < 0)
			cleaner(env, cmd, NULL, 1);
		cleaner(env, mochkil, files, 0);
	}
	wait_and_update_status(pid);
}

void	exec_single_cmd(t_cmd *cmd, t_env **env, t_heredoc_file *files)
{
	t_cmd	*mochkil;

	if (!cmd)
		return ;
	mochkil = cmd;
	if ((!cmd->arg || !cmd->arg[0]) && cmd->red)
	{
		handle_redirection_only(*env, cmd, mochkil, files);
		return ;
	}
	if (!cmd->arg || !cmd->arg[0])
	{
		g_global.exit_status = 0;
		return ;
	}
	update_underscore_variable(env, cmd->arg);
	if (is_builtin(cmd->arg[0]))
	{
		run_builtin_command(mochkil, cmd, env, files);
		return ;
	}
	exec_external_cmd(mochkil, cmd, env, files);
}
