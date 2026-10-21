/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:34:34 by schernan          #+#    #+#             */
/*   Updated: 2025/07/26 00:34:37 by schernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	parse_key_value_plus(const char *arg, char **key, char **value,
		int *is_append)
{
	char*(plus_equal), *(equal);
	*is_append = 0;
	if (!arg || !key || !value)
		return ;
	plus_equal = ft_strnstr(arg, "+=", ft_strlen(arg));
	if (plus_equal)
	{
		*is_append = 1;
		*key = ft_strndup(arg, plus_equal - arg);
		*value = ft_strdup(plus_equal + 2);
	}
	else
	{
		equal = ft_strchr(arg, '=');
		if (equal)
		{
			*key = ft_strndup(arg, equal - arg);
			*value = ft_strdup(equal + 1);
		}
		else
		{
			*key = ft_strdup(arg);
			*value = NULL;
		}
	}
}

int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 0;
	while (str[i] && str[i] != '=' && !(str[i] == '+' && str[i + 1] == '='))
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	if (str[i] == '+' && str[i + 1] != '=')
		return (0);
	return (1);
}

void	print_export(t_env *env)
{
	char	**arr;

	arr = env_to_export_array(env);
	if (!arr)
		return ;
	sort_env_array(arr);
	print_and_free_env_array(arr);
}

void	add_or_update_env_plus(t_env **env_list, const char *arg)
{
	char	*key;
	char	*value;
	t_env	*cur;
	int		is_append;
	void	*all[2];

	is_append = 0;
	key = NULL;
	value = NULL;
	is_append = 0;
	if (!arg || !env_list)
		return ;
	parse_key_value_plus(arg, &key, &value, &is_append);
	if (!key)
		return ;
	cur = *env_list;
	all[0] = key;
	all[1] = value;
	handle_env_update(env_list, cur, all, is_append);
}

int	ft_export(char **args, t_env **env_list)
{
	int (i), (status);
	i = 1;
	status = 0;
	if (!args || !env_list || !*env_list)
		return (1);
	if (!args[1])
	{
		print_export(*env_list);
		return (0);
	}
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			print_error(args[i], ": not a valid identifier\n");
			status = 1;
			g_global.exit_status = 1;
		}
		else
			add_or_update_env_plus(env_list, args[i]);
		i++;
	}
	return (status);
}
