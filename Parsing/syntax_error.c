/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:32:46 by schernan          #+#    #+#             */
/*   Updated: 2025/07/26 00:32:48 by schernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	syntax_error(char *msg)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	g_global.exit_status = 2;
}

bool	check_redirection_syntax(t_token *t)
{
	if (!t->next)
	{
		syntax_error("newline");
		return (false);
	}
	if (t->next->type != WORD)
	{
		if (t->next->value)
			syntax_error(t->next->value);
		else
			syntax_error("newline");
		return (false);
	}
	return (true);
}

bool	check_token_syntax(t_token *t)
{
	if (!t)
		return (true);
	if (t->type == PIPE)
	{
		if (!t->next || t->next->type == PIPE)
		{
			syntax_error("|");
			return (false);
		}
	}
	else if (t->type == INPUT || t->type == HEREDOC || t->type == OUTPUT
		|| t->type == APPEND)
		return (check_redirection_syntax(t));
	return (true);
}

bool	check_syntax(t_token *current)
{
	while (current)
	{
		if (!check_token_syntax(current))
			return (false);
		current = current->next;
	}
	return (true);
}

bool	validate_syntax(t_token *tokens)
{
	t_token	*last;

	if (!tokens)
		return (true);
	if (tokens->type == PIPE)
	{
		syntax_error("|");
		return (false);
	}
	last = tokens;
	while (last->next)
		last = last->next;
	if (last->type == PIPE)
	{
		syntax_error("|");
		return (false);
	}
	return (check_syntax(tokens));
}
