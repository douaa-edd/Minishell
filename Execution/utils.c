/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddaidi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:23:41 by deddaidi          #+#    #+#             */
/*   Updated: 2025/07/26 00:23:45 by deddaidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sort_env_array(char **arr)
{
	int		size;
	char	*tmp;

	int (i), (j);
	size = 0;
	if (!arr)
		return ;
	while (arr[size])
		size++;
	i = 0;
	while (i < size - 1)
	{
		j = i + 1;
		while (j < size)
		{
			if (ft_strcmp(arr[i], arr[j]) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	print_and_free_env_array(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		printf("declare -x %s\n", arr[i]);
		free(arr[i]);
		i++;
	}
	free(arr);
}

bool	is_delimiter(char *line, const char *delimiter)
{
	if (!line)
	{
		printf("minishell: warning: here-document delimited \
			by end-of-file (wanted `%s`)\n",
			delimiter);
		return (true);
	}
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (true);
	}
	return (false);
}

void	free_heredoc_files(t_heredoc_file *head)
{
	t_heredoc_file	*tmp;

	while (head)
	{
		unlink(head->filename);
		free(head->filename);
		tmp = head;
		head = head->next;
		free(tmp);
	}
}
