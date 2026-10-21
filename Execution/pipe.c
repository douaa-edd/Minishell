/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddaidi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:22:38 by deddaidi          #+#    #+#             */
/*   Updated: 2025/07/26 00:22:44 by deddaidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	child_process(t_cmd *mochkil, t_cmd *cmd, t_env **env, void **all)
{
	t_heredoc_file	*files;
	int				status;

	files = (t_heredoc_file *)all[2];
	g_global.is_child = 1;
	reset_signals();
	handle_redir_and_pipes(cmd, env, all);
	if (apply_redirections(cmd->red) < 0)
		cleaner(*env, mochkil, files, 1);
	status = exec_command(mochkil, cmd, *env, files);
	cleaner(*env, mochkil, files, status);
}

void	parent_process(int *prev_read_fd, int pipe_fd[2], t_cmd *has_next)
{
	if (*prev_read_fd != -1)
	{
		close(*prev_read_fd);
		*prev_read_fd = -1;
	}
	if (has_next)
	{
		close(pipe_fd[1]);
		*prev_read_fd = pipe_fd[0];
	}
	else
	{
		if (pipe_fd[0] != -1)
			close(pipe_fd[0]);
		if (pipe_fd[1] != -1)
			close(pipe_fd[1]);
	}
}

pid_t	setup_and_fork(t_cmd *cmd, t_env **env, void **all)
{
	int		*pipe_fd;
	pid_t	pid;

	pipe_fd = (int *)all[1];
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	if (cmd->next && pipe(pipe_fd) == -1)
	{
		perror("pipe");
		cleaner(*env, cmd, NULL, EXIT_FAILURE);
	}
	if (cmd->arg && cmd->arg[0])
		update_underscore_variable(env, cmd->arg);
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		cleaner(*env, cmd, NULL, EXIT_FAILURE);
	}
	return (pid);
}

void	handle_one_command(t_cmd *mochkil, t_cmd *cmd, t_env **env, void **all)
{
	int		*prev_read_fd;
	int		*pipe_fd;
	pid_t	pid;

	prev_read_fd = (int *)all[0];
	pipe_fd = (int *)all[1];
	pid = setup_and_fork(cmd, env, all);
	if (pid == 0)
		child_process(mochkil, cmd, env, all);
	else
		parent_process(prev_read_fd, pipe_fd, cmd->next);
}

void	exec_pipeline(t_cmd *cmds, t_env **env, t_heredoc_file *files)
{
	int		prev_read_fd;
	int		last_status;
	int		pipe_fd[2];
	void	*all[3];
	t_cmd	*cmd;

	prev_read_fd = -1;
	last_status = 0;
	all[0] = &prev_read_fd;
	all[1] = pipe_fd;
	all[2] = files;
	cmd = cmds;
	while (cmd)
	{
		handle_one_command(cmds, cmd, env, all);
		cmd = cmd->next;
	}
	if (prev_read_fd != -1)
		close(prev_read_fd);
	wait_all_children(&last_status);
}
