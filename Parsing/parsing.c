/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:32:24 by schernan          #+#    #+#             */
/*   Updated: 2025/07/26 00:32:26 by schernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**ft_realloc(char *arg, char **old_arr)
{
	char	**new_arr;

	int (i), (size);
	if (!arg)
		return (NULL);
	i = 0;
	size = arr_size(old_arr);
	new_arr = malloc(sizeof(char *) * (size + 2));
	if (!new_arr)
	{
		free(arg);
		return (NULL);
	}
	if (old_arr)
	{
		while (old_arr[i])
		{
			new_arr[i] = old_arr[i];
			i++;
		}
		free(old_arr);
	}
	new_arr[i++] = arg;
	new_arr[i] = NULL;
	return (new_arr);
}

int	append_arg(t_cmd *cmd, const char *value)
{
	char	*arg_dup;
	char	**tmp_args;

	if (!value || !cmd)
		return (0);
	arg_dup = ft_strdup(value);
	if (!arg_dup)
	{
		g_global.exit_status = 1;
		return (0);
	}
	tmp_args = ft_realloc(arg_dup, cmd->arg);
	if (!tmp_args)
	{
		free(arg_dup);
		if (cmd->arg)
			free_split(cmd->arg);
		cmd->arg = NULL;
		g_global.exit_status = 1;
		return (0);
	}
	cmd->arg = tmp_args;
	return (1);
}

int	fill_command_parts(t_cmd *cmd, t_token **tokens)
{
	t_token	*tok;

	while (*tokens && (*tokens)->type != PIPE)
	{
		tok = *tokens;
		if (tok->type == WORD)
		{
			if (!append_arg(cmd, tok->value))
				return (free_cmds(cmd), 0);
			*tokens = tok->next;
		}
		else if (tok->type == INPUT || tok->type == HEREDOC
			|| tok->type == OUTPUT || tok->type == APPEND)
		{
			if (!handle_red(cmd, tokens))
			{
				free_cmds(cmd);
				return (0);
			}
		}
		else
			*tokens = tok->next;
	}
	return (1);
}

t_cmd	*create_command(t_token **tokens)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
	{
		g_global.exit_status = 1;
		return (NULL);
	}
	cmd->arg = NULL;
	cmd->red = NULL;
	cmd->next = NULL;
	if (!fill_command_parts(cmd, tokens))
	{
		free_cmd(cmd);
		return (NULL);
	}
	if (*tokens && (*tokens)->type == PIPE)
		*tokens = (*tokens)->next;
	return (cmd);
}

t_cmd	*build_commands(t_token *tokens)
{
	t_cmd	*head;
	t_cmd	*current;
	t_cmd	*cmd;

	head = NULL;
	current = NULL;
	while (tokens)
	{
		cmd = create_command(&tokens);
		if (!cmd)
		{
			free_cmds(head);
			return (NULL);
		}
		if (!head)
			head = cmd;
		else
			current->next = cmd;
		current = cmd;
	}
	return (head);
}
