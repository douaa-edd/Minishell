/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_paths.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:34:59 by schernan          #+#    #+#             */
/*   Updated: 2025/07/26 00:35:01 by schernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_env_vars(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

char	**env_to_array(t_env *env)
{
	int		i;
	char	**envp;

	envp = malloc(sizeof(char *) * (count_env_vars(env) + 1));
	if (!envp)
		return (NULL);
	i = 0;
	while (env)
	{
		envp[i] = ft_strjoin_three(env->key, "=", env->value);
		if (!envp[i])
			return (free_env_array(envp, i), NULL);
		i++;
		env = env->next;
	}
	envp[i] = NULL;
	return (envp);
}

void	handle_no_path(char *cmd, t_env *env, t_cmd *mochkil,
		t_heredoc_file *files)
{
	if (ft_strchr(cmd, '/'))
		print_error(cmd, ": No such file or directory\n");
	else
		print_error(cmd, ": command not found\n");
	cleaner(env, mochkil, files, 127);
}

char	*check_path_errors(char *path, char *cmd, void **all)
{
	t_cmd			*mochkil;
	t_cmd			*cmds;
	t_env			*env;
	t_heredoc_file	*files;

	mochkil = (t_cmd *)all[0];
	cmds = (t_cmd *)all[1];
	env = (t_env *)all[2];
	files = (t_heredoc_file *)all[3];
	if (!path)
		handle_no_path(cmd, env, mochkil, files);
	if (is_directory(path))
	{
		print_error(path, ": Is a directory\n");
		free(path);
		g_global.exit_status = 126;
		return (NULL);
	}
	if (access(path, X_OK) != 0)
	{
		print_error(cmd, ": Permission denied\n");
		free(path);
		cleaner(env, cmds, files, 126);
	}
	return (path);
}

char	*prepare_exec(t_cmd *cmd, t_env **env, char ***envp, void **all)
{
	char	*path;

	if (!cmd->arg[0] || cmd->arg[0][0] == '\0')
	{
		ft_putstr_fd("minishell: command not found\n", 2);
		cleaner(*env, cmd, NULL, 127);
	}
	path = find_executable(cmd->arg[0], *env);
	path = check_path_errors(path, cmd->arg[0], all);
	if (!path)
		return (NULL);
	*envp = env_to_array(*env);
	return (path);
}
