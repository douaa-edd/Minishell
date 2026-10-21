/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddaidi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:22:25 by deddaidi          #+#    #+#             */
/*   Updated: 2025/07/26 00:22:29 by deddaidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_heredoc_parent(int status, char *filename, t_red *redir,
		t_exec_context *ctx)
{
	t_heredoc_file	*new_node;

	if (WIFSIGNALED(status))
	{
		g_global.exit_status = 128 + WTERMSIG(status);
		write(1, "\n", 1);
		unlink(filename);
		free(filename);
		return (1);
	}
	g_global.exit_status = WEXITSTATUS(status);
	new_node = malloc(sizeof(t_heredoc_file));
	if (!new_node)
	{
		unlink(filename);
		free(filename);
		return (-1);
	}
	new_node->filename = filename;
	new_node->next = ctx->files;
	ctx->files = new_node;
	free(redir->file);
	redir->file = ft_strdup(filename);
	return (0);
}

int	process_heredoc_utils(t_red *redir, t_env *env_list, char *filename)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	if (read_heredoc(redir->file, redir->quote_type, filename, env_list) < 0)
		return (1);
	return (0);
}

char	*make_temp_filename(void)
{
	static int	counter = 1;
	char		*prefix;
	char		*num;
	char		*filename;

	prefix = "heredoc_";
	num = ft_itoa(++counter);
	if (!num)
		return (NULL);
	filename = ft_strjoin(prefix, num);
	free(num);
	if (!filename)
		return (NULL);
	return (filename);
}

int	process_heredoc(t_red *redir, t_env *env_list, t_exec_context *ctx,
		t_cmd *cmd_list)
{
	char	*filename;
	pid_t	pid;

	int (r), (status);
	filename = make_temp_filename();
	if (!filename)
		return (-1);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free(filename);
		return (-1);
	}
	if (pid == 0)
	{
		g_global.is_child = 1;
		r = process_heredoc_utils(redir, env_list, filename);
		free(filename);
		cleaner(env_list, cmd_list, ctx->files, r);
	}
	waitpid(pid, &status, 0);
	signal(SIGINT, handle_sigint);
	return (handle_heredoc_parent(status, filename, redir, ctx));
}

int	check_heredoc(t_exec_context *ctx, t_cmd *cmd_list, t_env *env_list)
{
	t_cmd	*cmd;
	int		r;
	t_red	*redir;

	cmd = cmd_list;
	while (cmd)
	{
		redir = cmd->red;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				r = process_heredoc(redir, env_list, ctx, cmd_list);
				if (r != 0)
					return (r);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
