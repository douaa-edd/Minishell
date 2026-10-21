/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:36:11 by schernan          #+#    #+#             */
/*   Updated: 2025/07/26 00:36:13 by schernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strjoin_three(const char *s1, const char *s2, const char *s3)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, s3);
	free(tmp);
	return (res);
}

void	print_error(char *msg, char *error)
{
	if (msg)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(msg, 2);
		ft_putstr_fd(error, 2);
	}
}

int	wait_and_update_status(pid_t pid)
{
	int	status;

	if (pid < 0)
	{
		perror("fork");
		g_global.exit_status = 1;
		return (1);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_global.exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		g_global.exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			write(1, "Quit(core dumped)\n", 19);
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
	}
	return (0);
}

int	update_env_value(t_env *env_list, const char *key, const char *value)
{
	t_env	*cur;

	if (!key)
		return (0);
	cur = env_list;
	while (cur)
	{
		if (cur->key && ft_strcmp(cur->key, key) == 0)
		{
			free(cur->value);
			if (value)
				cur->value = ft_strdup(value);
			else
				cur->value = NULL;
			return (1);
		}
		cur = cur->next;
	}
	return (0);
}

void	update_underscore_variable(t_env **env, char **args)
{
	int	i;

	if (!args || !args[0])
		return ;
	i = 0;
	while (args[i])
		i++;
	if (i > 0)
		update_env_value(*env, "_", args[i - 1]);
}
