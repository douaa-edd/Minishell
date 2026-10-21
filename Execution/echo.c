/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:33:55 by schernan          #+#    #+#             */
/*   Updated: 2025/07/26 00:33:57 by schernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_arg(char **arg, int i)
{
	int	first;
	int	j;

	if (!arg)
		return ;
	first = 1;
	j = i;
	while (arg[j])
	{
		if (!first)
			printf(" ");
		printf("%s", arg[j]);
		first = 0;
		j++;
	}
}

int	ft_echo(char **arg)
{
	int	i;
	int	print_newline;
	int	j;

	if (!arg)
		return (1);
	i = 1;
	print_newline = 1;
	while (arg[i] && arg[i][0] == '-' && arg[i][1] == 'n')
	{
		j = 2;
		while (arg[i][j] == 'n')
			j++;
		if (arg[i][j] != '\0')
			break ;
		print_newline = 0;
		i++;
	}
	print_arg(arg, i);
	if (print_newline)
		printf("\n");
	return (0);
}
