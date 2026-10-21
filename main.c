/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddaidi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:18:28 by deddaidi          #+#    #+#             */
/*   Updated: 2025/07/26 00:18:33 by deddaidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_global	g_global = {.exit_status = 0, .is_child = 0};

// char	*get_token_type_name(t_token_type type)
// {
// 	if (type == WORD)
// 		return ("WORD");
// 	if (type == PIPE)
// 		return ("PIPE");
// 	if (type == INPUT)
// 		return ("INPUT");
// 	if (type == OUTPUT)
// 		return ("OUTPUT");
// 	if (type == APPEND)
// 		return ("APPEND");
// 	if (type == HEREDOC)
// 		return ("HEREDOC");
// 	return ("UNKNOWN");
// }

// char	*get_quote_type_name(t_quote q)
// {
// 	if (q == NONE)
// 		return ("NONE");
// 	if (q == SINGLE)
// 		return ("SINGLE");
// 	if (q == DOUBLE)
// 		return ("DOUBLE");
// 	return ("UNKNOWN");
// }

// void	print_tokens(t_token *tokens)
// {
// 	while (tokens)
// 	{
// 		printf("value: [%s], type: %s, quote: %s\n", tokens->value,
// 			get_token_type_name(tokens->type),
// 			get_quote_type_name(tokens->quote_type));
// 		tokens = tokens->next;
// 	}
// }

// const char	*redir_type_name(t_redir_type type)
// {
// 	if (type == REDIR_IN)
// 		return ("<");
// 	if (type == REDIR_OUT)
// 		return (">");
// 	if (type == REDIR_APPEND)
// 		return (">>");
// 	if (type == REDIR_HEREDOC)
// 		return ("<<");
// 	return ("?");
// }

// void	print_cmds(t_cmd *cmd)
// {
// 	int		i;
// 	t_red	*red;

// 	while (cmd)
// 	{
// 		printf("Command:\n");
// 		if (cmd->arg)
// 		{
// 			printf("  Arguments: ");
// 			for (i = 0; cmd->arg[i]; i++)
// 				printf("\"%s\" ", cmd->arg[i]);
// 			printf("\n");
// 		}
// 		else
// 			printf("  Arguments: (none)\n");
// 		printf("  Redirections:\n");
// 		red = cmd->red;
// 		if (!red)
// 			printf("    (none)\n");
// 		while (red)
// 		{
// 			printf("    %s %s\n", redir_type_name(red->type), red->file);
// 			red = red->next;
// 		}
// 		printf("\n");
// 		cmd = cmd->next;
// 	}
// }

/////////////////////////////////////////////////////

int	parse_and_validate(t_env *env_list, char *input, t_cmd **cmds_ptr)
{
	t_token	*tokens;

	tokens = tokenize_line(input);
	if (!tokens)
		return (0);
	expand_tokens(&tokens, env_list);
	split_token_on_spaces(&tokens);
	if (!validate_syntax(tokens))
	{
		free_tokens(tokens);
		return (0);
	}
	*cmds_ptr = build_commands(tokens);
	free_tokens(tokens);
	if (!*cmds_ptr)
	{
		write(STDERR_FILENO, "minishell: failed to build commands\n", 36);
		return (0);
	}
	return (1);
}

int	handle_command_line(t_env **env_list, char *input)
{
	t_exec_context	ctx;
	t_cmd			*cmds;

	if (!parse_and_validate(*env_list, input, &cmds))
		return (0);
	ctx.files = NULL;
	if (check_heredoc(&ctx, cmds, *env_list) != 0)
	{
		free_cmds(cmds);
		free_heredoc_files(ctx.files);
		ctx.files = NULL;
		return (0);
	}
	if (cmds->next)
		exec_pipeline(cmds, env_list, ctx.files);
	else
		exec_single_cmd(cmds, env_list, ctx.files);
	free_cmds(cmds);
	free_heredoc_files(ctx.files);
	ctx.files = NULL;
	return (1);
}

int	input_process(t_env **env_list)
{
	char	*input;
	int		ret;

	input = readline("minishell$ ");
	if (!input)
	{
		printf("exit\n");
		return (-1);
	}
	if (*input)
		add_history(input);
	ret = handle_command_line(env_list, input);
	free(input);
	return (ret);
}

int	main(int ac, char **av, char **envp)
{
	t_env	*env_list;

	(void)ac;
	(void)av;
	env_list = init_env(envp);
	increment_shlvl(&env_list);
	init_signals();
	while (1)
	{
		if (input_process(&env_list) < 0)
			break ;
	}
	free_env(env_list);
	close_all_fds();
	return (g_global.exit_status);
}
