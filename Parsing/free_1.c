/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddaidi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:19:50 by deddaidi          #+#    #+#             */
/*   Updated: 2025/07/26 00:19:52 by deddaidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
}

void	free_redirections(t_red *red)
{
	t_red	*tmp;

	while (red)
	{
		tmp = red->next;
		if (red->file)
			free(red->file);
		free(red);
		red = tmp;
	}
}

void	free_cmds(t_cmd *cmd)
{
	t_cmd	*tmp;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->arg)
		{
			free_args(cmd->arg);
			cmd->arg = NULL;
		}
		if (cmd->red)
		{
			free_redirections(cmd->red);
			cmd->red = NULL;
		}
		free(cmd);
		cmd = tmp;
	}
}

void	close_all_fds(void)
{
	int	fd;

	fd = 3;
	while (fd < 1024)
	{
		close(fd);
		fd++;
	}
}

void	cleaner(t_env *env_list, t_cmd *cmds, t_heredoc_file *files, int status)
{
	close_all_fds();
	if (env_list)
		free_env(env_list);
	if (cmds)
		free_cmds(cmds);
	if (files)
		free_heredoc_files(files);
	exit(status);
}
