/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   red.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddaidi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:22:54 by deddaidi          #+#    #+#             */
/*   Updated: 2025/07/26 00:23:02 by deddaidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_dup(int fd, t_redir_type type)
{
	if (type == REDIR_IN || type == REDIR_HEREDOC)
	{
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			perror("dup2 stdin");
			close(fd);
			return (-1);
		}
	}
	else if (type == REDIR_OUT || type == REDIR_APPEND)
	{
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			perror("dup2 stdout");
			close(fd);
			return (-1);
		}
	}
	else
	{
		close(fd);
		write(STDERR_FILENO, "handle_dup: unknown redirection type\n", 37);
		return (-1);
	}
	return (0);
}

int	open_redirection_file(t_red *red)
{
	if (red->type == REDIR_IN || red->type == REDIR_HEREDOC)
		return (open(red->file, O_RDONLY));
	if (red->type == REDIR_OUT)
		return (open(red->file, O_CREAT | O_WRONLY | O_TRUNC, 0644));
	if (red->type == REDIR_APPEND)
		return (open(red->file, O_CREAT | O_WRONLY | O_APPEND, 0644));
	write(STDERR_FILENO, "apply_redirections: unknown redirection type\n", 46);
	return (-1);
}

int	apply_redirections(t_red *red)
{
	int	fd;

	while (red)
	{
		if (!red->file)
		{
			write(STDERR_FILENO,
				"apply_redirections: redirection file is NULL\n", 45);
			return (-1);
		}
		fd = open_redirection_file(red);
		if (fd < 0)
			return (perror(red->file), -1);
		if (handle_dup(fd, red->type) < 0)
			return (close(fd), -1);
		close(fd);
		red = red->next;
	}
	return (0);
}
