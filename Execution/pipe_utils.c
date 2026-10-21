/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:35:36 by schernan          #+#    #+#             */
/*   Updated: 2025/07/26 00:35:37 by schernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exec_command(t_cmd *mochkil, t_cmd *cmd, t_env *env, t_heredoc_file *files)
{
	if ((!cmd->arg || !cmd->arg[0] || cmd->arg[0][0] == '\0') && !cmd->red)
	{
		write(STDERR_FILENO, "minishell: command not found\n", 29);
		return (127);
	}
	if (!cmd->arg || !cmd->arg[0] || cmd->arg[0][0] == '\0')
		return (0);
	if (is_builtin(cmd->arg[0]))
		return (exec_builtin(mochkil, cmd, &env, files));
	return (exec_external_cmd(mochkil, cmd, &env, files));
}

void	handle_redir_and_pipes(t_cmd *cmd, t_env **env, void **all)
{
	int	*prev_read_fd;
	int	*pipe_fd;

	prev_read_fd = (int *)all[0];
	pipe_fd = (int *)all[1];
	if (*prev_read_fd != -1)
	{
		if (dup2(*prev_read_fd, STDIN_FILENO) < 0)
		{
			perror("dup2 stdin");
			cleaner(*env, cmd, NULL, 1);
		}
		close(*prev_read_fd);
	}
	if (cmd->next)
	{
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) < 0)
		{
			perror("dup2 stdout");
			close(pipe_fd[1]);
			cleaner(*env, cmd, NULL, 1);
		}
		close(pipe_fd[1]);
	}
}

void	wait_all_children(int *last_status)
{
	pid_t	wpid;
	int		status;

	wpid = wait(&status);
	while (wpid > 0)
	{
		*last_status = status;
		wpid = wait(&status);
	}
	if (WIFEXITED(*last_status))
		g_global.exit_status = WEXITSTATUS(*last_status);
	else if (WIFSIGNALED(*last_status))
	{
		g_global.exit_status = 128 + WTERMSIG(*last_status);
		write(1, "\n", 1);
	}
	g_global.is_child = 0;
	init_signals();
}
