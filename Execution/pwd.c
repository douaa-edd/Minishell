/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:35:48 by schernan          #+#    #+#             */
/*   Updated: 2025/07/26 00:35:49 by schernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_pwd(t_env *env_list)
{
	char	*cwd;
	char	*pwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		pwd = get_env_value(env_list, "PWD");
		if (pwd)
			printf("%s\n", pwd);
		else
		{
			write(STDERR_FILENO,
				"pwd: error retrieving current directory: getcwd failed\n", 55);
			g_global.exit_status = 1;
			return (1);
		}
	}
	else
	{
		printf("%s\n", cwd);
		free(cwd);
	}
	g_global.exit_status = 0;
	return (0);
}
