/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:34:13 by schernan          #+#    #+#             */
/*   Updated: 2025/07/26 00:34:14 by schernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_digits(char *arg)
{
	int	i;

	if (!arg || arg[0] == '\0')
		return (-1);
	i = 0;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	if (!arg[i])
		return (-1);
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (-1);
		i++;
	}
	return (ft_atoi(arg));
}

int	ft_exit(char **arg, bool in_parent, void **all, t_env *env_list)
{
	int				status;
	t_cmd			*mochkil;
	t_heredoc_file	*files;

	mochkil = (t_cmd *)all[0];
	files = (t_heredoc_file *)all[1];
	if (in_parent)
		printf("exit\n");
	if (!arg || !arg[1])
		cleaner(env_list, mochkil, files, 0);
	status = is_digits(arg[1]);
	if (status == -1)
	{
		write(STDERR_FILENO, "exit: numeric argument required\n", 32);
		cleaner(env_list, mochkil, files, 2);
	}
	if (arg[2])
	{
		write(STDERR_FILENO, "exit: too many arguments\n", 26);
		return (1);
	}
	cleaner(env_list, mochkil, files, status % 256);
	return (0);
}
