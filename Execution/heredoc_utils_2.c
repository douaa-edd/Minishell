/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:35:24 by schernan          #+#    #+#             */
/*   Updated: 2025/07/26 00:35:26 by schernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_heredoc_line(char *line, const char *delimiter, int fd,
		bool write_line)
{
	if (!write_line || ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (0);
	}
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
	return (1);
}

bool	is_delim_fully_quoted(const char *str)
{
	int	len;

	len = ft_strlen(str);
	if (len < 2)
		return (false);
	return ((str[0] == '\'' && str[len - 1] == '\'') || (str[0] == '"'
			&& str[len - 1] == '"'));
}

void	append_char(char **result, char c)
{
	char	tmp[2];
	char	*old;
	char	*joined;

	if (!result)
		return ;
	tmp[0] = c;
	tmp[1] = '\0';
	old = *result;
	joined = ft_strjoin(*result, tmp);
	if (!joined)
	{
		free(old);
		*result = NULL;
		return ;
	}
	*result = joined;
	free(old);
}

int	append_variable(const char *line, int i, char **result, t_env *env)
{
	int		start;
	char	*var_name;
	char	*var_value;
	char	*old;

	start = i;
	while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
		i++;
	var_name = ft_substr(line, start, i - start);
	var_value = get_env_value(env, var_name);
	free(var_name);
	if (var_value)
	{
		old = *result;
		*result = ft_strjoin(*result, var_value);
		free(old);
	}
	return (i);
}

char	*expand_heredoc_line(const char *line, t_env *env)
{
	int		i;
	char	*result;

	i = 0;
	result = ft_strdup("");
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] && (ft_isalnum(line[i + 1]) || line[i
					+ 1] == '_'))
		{
			i = append_variable(line, i + 1, &result, env);
		}
		else
		{
			append_char(&result, line[i]);
			i++;
		}
	}
	return (result);
}
