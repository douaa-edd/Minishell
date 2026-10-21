/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddaidi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:21:14 by deddaidi          #+#    #+#             */
/*   Updated: 2025/07/26 00:21:16 by deddaidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_pwd_node(t_env **env_list, char *cwd)
{
	t_env	*pwd_node;

	pwd_node = get_env_node(*env_list, "PWD");
	if (pwd_node)
	{
		if (pwd_node->value)
			update_oldpwd(env_list, pwd_node->value);
		else
			update_oldpwd(env_list, "");
		free(pwd_node->value);
		pwd_node->value = ft_strdup(cwd);
	}
	else
	{
		add_env_node(env_list, new_env_node("PWD", cwd));
		update_oldpwd(env_list, "");
	}
}

void	update_pwd_env(t_env **env_list)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		write(2,
			"cd: error retrieving current directory: getcwd: "
			"cannot access parent directories: No such file or directory\n",
			108);
		g_global.exit_status = 0;
		return ;
	}
	update_pwd_node(env_list, cwd);
	free(cwd);
}

char	*get_target_path(char **args, t_env *home, t_env *oldpwd)
{
	if (args[1] && ft_strcmp(args[1], "-") == 0)
	{
		if (!oldpwd || !oldpwd->value)
			return (NULL);
		printf("%s\n", oldpwd->value);
		return (oldpwd->value);
	}
	else if (!args[1])
	{
		if (!home || !home->value)
			return (NULL);
		return (home->value);
	}
	else
		return (args[1]);
}

int	check_cd_errors(char **args)
{
	if (args[1] && args[2])
	{
		cd_error(0);
		return (1);
	}
	if (args[1] && args[1][0] == '\0')
	{
		g_global.exit_status = 1;
		return (1);
	}
	return (0);
}

int	ft_cd(char **args, t_env **env_list)
{
	t_env	*home_node;
	t_env	*oldpwd_node;
	char	*target_path;

	if (!env_list)
		return (1);
	if (check_cd_errors(args))
		return (1);
	home_node = get_env_node(*env_list, "HOME");
	oldpwd_node = get_env_node(*env_list, "OLDPWD");
	target_path = get_target_path(args, home_node, oldpwd_node);
	if (!target_path)
	{
		if (ft_strcmp(args[1], "-") == 0)
			cd_error(3);
		else
			cd_error(1);
		return (1);
	}
	if (chdir(target_path) != 0)
		return (cd_error(2), 1);
	g_global.exit_status = 0;
	update_pwd_env(env_list);
	return (0);
}
