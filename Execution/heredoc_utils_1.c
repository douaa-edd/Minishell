/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddaidi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:22:12 by deddaidi          #+#    #+#             */
/*   Updated: 2025/07/26 00:22:14 by deddaidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*strip_quotes(const char *str)
{
	int	start;
	int	end;

	start = 0;
	end = ft_strlen(str) - 1;
	while (str[start] && (str[start] == '\'' || str[start] == '"'))
		start++;
	while (end > start && (str[end] == '\'' || str[end] == '"'))
		end--;
	return (ft_substr(str, start, end - start + 1));
}

bool	should_expand_delimiter(const char *delimiter)
{
	return (delimiter[0] == '$' && (delimiter[1] == '"'
			|| delimiter[1] == '\''));
}

void	process_heredoc_line(int fd, const char *clean_delim,
		t_quote quote_type, t_env *env)
{
	char	*line;
	char	*expanded;
	bool	expand_content;

	expand_content = !is_delim_fully_quoted(clean_delim);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, clean_delim) == 0)
		{
			free(line);
			break ;
		}
		if (expand_content && quote_type == NONE)
		{
			expanded = expand_heredoc_line(line, env);
			free(line);
			line = expanded;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

void	read_heredoc_loop(int fd, const char *delimiter, t_quote quote_type,
		t_env *env)
{
	char	*clean_delim;
	char	*tmp;
	char	*stripped;

	clean_delim = ft_strdup(delimiter);
	if (should_expand_delimiter(delimiter))
	{
		tmp = expand_variable_value(clean_delim, env);
		free(clean_delim);
		clean_delim = tmp;
	}
	stripped = strip_quotes(clean_delim);
	free(clean_delim);
	clean_delim = stripped;
	process_heredoc_line(fd, clean_delim, quote_type, env);
	free(clean_delim);
}

int	read_heredoc(const char *delimiter, t_quote quote_type, char *filename,
		t_env *env)
{
	int	fd;

	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd < 0)
	{
		perror("open");
		return (-1);
	}
	read_heredoc_loop(fd, delimiter, quote_type, env);
	close(fd);
	return (0);
}
