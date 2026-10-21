/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_paths_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddaidi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:21:54 by deddaidi          #+#    #+#             */
/*   Updated: 2025/07/26 00:21:56 by deddaidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_directory(const char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0)
		return (S_ISDIR(st.st_mode));
	return (0);
}

char	*search_paths(char *cmd, char **paths)
{
	int		i;
	char	*tmp;
	char	*full;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (!tmp)
			return (NULL);
		full = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!full)
			return (NULL);
		if (access(full, X_OK) == 0 && !is_directory(full))
			return (full);
		free(full);
		i++;
	}
	return (NULL);
}

char	*check_direct_path(char *cmd)
{
	char	*res;

	if (access(cmd, F_OK) != 0)
		return (g_global.exit_status = 127, NULL);
	if (access(cmd, X_OK) != 0)
		return (g_global.exit_status = 126, NULL);
	res = ft_strdup(cmd);
	if (!res)
		g_global.exit_status = 1;
	return (res);
}

char	*find_executable(char *cmd, t_env *env)
{
	char	*path_var;
	char	**paths;
	char	*res;

	if (!cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (check_direct_path(cmd));
	path_var = get_env_value(env, "PATH");
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	res = search_paths(cmd, paths);
	free_split(paths);
	if (!res)
		g_global.exit_status = 127;
	return (res);
}
