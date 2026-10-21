/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddaidi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:20:04 by deddaidi          #+#    #+#             */
/*   Updated: 2025/07/26 00:20:08 by deddaidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	arr_size(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return (0);
	while (arr[i])
		i++;
	return (i);
}

t_red	*create_red(t_redir_type type, const char *file, t_quote quote_type)
{
	t_red	*new;

	new = malloc(sizeof(t_red));
	if (!new)
		return (NULL);
	new->type = type;
	new->file = ft_strdup(file);
	if (!new->file)
	{
		free(new);
		return (NULL);
	}
	new->quote_type = quote_type;
	new->next = NULL;
	return (new);
}

void	add_red(t_cmd *cmd, t_redir_type type, const char *file,
		t_quote quote_type)
{
	t_red	*new;
	t_red	*cur;

	new = create_red(type, file, quote_type);
	if (!new)
	{
		g_global.exit_status = 1;
		return ;
	}
	if (!cmd->red)
		cmd->red = new;
	else
	{
		cur = cmd->red;
		while (cur->next)
			cur = cur->next;
		cur->next = new;
	}
}

t_redir_type	token_to_redir_type(t_token_type token_type)
{
	if (token_type == INPUT)
		return (REDIR_IN);
	else if (token_type == OUTPUT)
		return (REDIR_OUT);
	else if (token_type == APPEND)
		return (REDIR_APPEND);
	else if (token_type == HEREDOC)
		return (REDIR_HEREDOC);
	return (REDIR_IN);
}

int	handle_red(t_cmd *cmd, t_token **tokens)
{
	t_token			*tok;
	t_redir_type	redir_type;

	tok = *tokens;
	if (!tok->next || tok->next->type != WORD)
	{
		g_global.exit_status = 2;
		return (0);
	}
	redir_type = token_to_redir_type(tok->type);
	add_red(cmd, redir_type, tok->next->value, tok->next->quote_type);
	*tokens = tok->next->next;
	return (1);
}
