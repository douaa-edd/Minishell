/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schernan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 00:36:56 by schernan          #+#    #+#             */
/*   Updated: 2025/07/26 00:36:58 by schernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "Libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_global
{
	int						exit_status;
	int						is_child;
}							t_global;

extern t_global				g_global;

typedef enum e_token_type
{
	WORD,
	PIPE,
	INPUT,
	OUTPUT,
	APPEND,
	HEREDOC
}							t_token_type;

typedef enum e_quote
{
	NONE,
	SINGLE,
	DOUBLE
}							t_quote;

typedef struct s_token
{
	char					*value;
	t_token_type			type;
	t_quote					quote_type;
	struct s_token			*next;
}							t_token;

typedef struct s_env
{
	char					*key;
	char					*value;
	struct s_env			*next;
}							t_env;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}							t_redir_type;

typedef struct s_red
{
	t_redir_type			type;
	char					*file;
	t_quote					quote_type;
	struct s_red			*next;
}							t_red;

typedef struct s_cmd
{
	char					**arg;
	t_red					*red;
	struct s_cmd			*next;
}							t_cmd;

typedef struct s_heredoc_file
{
	char					*filename;
	struct s_heredoc_file	*next;
}							t_heredoc_file;

typedef struct s_exec_context
{
	t_heredoc_file			*files;
}							t_exec_context;

// Initialization functions
t_env						*init_env(char **envp);
void						increment_shlvl(t_env **env_list);
void						init_signals(void);
void						reset_signals(void);
void						handle_sigint(int sig);

// Parsing functions
t_token						*tokenize_line(char *input);
char						*process_input(char *input, int *i,
								t_quote *quote_type);
void						split_token_on_spaces(t_token **tokens_ptr);
void						free_split(char **arr);
t_token						*new_token(char *value, t_token_type type,
								t_quote quote_type);

void						expand_tokens(t_token **tokens_ptr, t_env *env);
char						*case_of_var_with_next_char_digit(char *word,
								int *i, char *result);
char						*case_of_var_with_next_char_quote(char *word,
								int *i, char *result, char delimiter);
char						*case_of_word(char *word, int *i, char *result);
char						*case_of_dquote(char *word, int *i, char *result,
								t_env *env);
char						*case_of_squote(char *word, int *i, char *result);
char						*case_of_var_with_exit_status(int *i, char *result);
char						*case_of_normal_var(char *word, int *i,
								char *result, t_env *env);
char						*get_env_value(t_env *env, char *key);
char						*ft_strjoin_char(char *str, char c);
char						*expand_variable_value(char *word, t_env *env);

bool						validate_syntax(t_token *tokens);

t_cmd						*build_commands(t_token *tokens);
int							arr_size(char **arr);
int							handle_red(t_cmd *cmd, t_token **tokens);

// Execution functions
void						exec_single_cmd(t_cmd *cmd, t_env **env,
								t_heredoc_file *files);
void						update_underscore_variable(t_env **env,
								char **args);
int							wait_and_update_status(pid_t pid);
void						print_error(char *msg, char *error);
char						*ft_strjoin_three(const char *s1, const char *s2,
								const char *s3);

char						*prepare_exec(t_cmd *cmd, t_env **env, char ***envp,
								void **all);
char						*find_executable(char *cmd, t_env *env);
int							is_directory(const char *path);
int							count_env_vars(t_env *env);

int							apply_redirections(t_red *red);
void						exec_pipeline(t_cmd *cmds, t_env **env,
								t_heredoc_file *files);
int							exec_external_cmd(t_cmd *mochkil, t_cmd *cmd,
								t_env **env, t_heredoc_file *files);
void						handle_redir_and_pipes(t_cmd *cmd, t_env **env,
								void **all);
int							exec_command(t_cmd *mochkil, t_cmd *cmd, t_env *env,
								t_heredoc_file *files);
void						wait_all_children(int *last_status);

// Built-in functions
int							exec_builtin(t_cmd *mochkil, t_cmd *cmd,
								t_env **env, t_heredoc_file *files);
int							is_builtin(char *cmd);
int							ft_cd(char **args, t_env **env_list);
void						cd_error(int code);
t_env						*get_env_node(t_env *env_list, const char *key);
void						add_env_node(t_env **env_list, t_env *new_node);
t_env						*new_env_node(const char *key, const char *value);
void						update_oldpwd(t_env **env_list,
								const char *old_pwd);
int							ft_export(char **args, t_env **env_list);
void						handle_env_update(t_env **env_list, t_env *cur,
								void **all, int is_append);
char						**env_to_export_array(t_env *env);
void						print_and_free_env_array(char **arr);
void						sort_env_array(char **arr);
int							ft_pwd(t_env *env_list);
int							ft_unset(char **args, t_env **env_list);
int							ft_echo(char **arg);
int							ft_env(t_cmd *cmd, t_env *env);
int							ft_exit(char **arg, bool in_parent, void **all,
								t_env *env_list);

// Heredoc functions
int							check_heredoc(t_exec_context *ctx, t_cmd *cmd_list,
								t_env *env_list);
int							read_heredoc(const char *delimiter,
								t_quote quote_type, char *filename, t_env *env);
char						*expand_heredoc_line(const char *line, t_env *env);
void						free_heredoc_files(t_heredoc_file *head);
bool						is_delimiter(char *line, const char *delimiter);
bool						is_delim_fully_quoted(const char *str);

// Freeing functions
void						free_tokens(t_token *head);
void						free_cmd(t_cmd *cmd);
void						free_cmds(t_cmd *cmd);
void						free_env(t_env *env);
void						free_env_array(char **envp, int size);
void						free_heredoc_files(t_heredoc_file *head);
void						free_envp(char **envp);
void						close_all_fds(void);
void						cleaner(t_env *env_list, t_cmd *cmds,
								t_heredoc_file *files, int status);

#endif
