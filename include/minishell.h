/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 12:39:10 by jmarques          #+#    #+#             */
/*   Updated: 2026/02/17 10:40:35 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ============================== INCLUDES ============================== */

# include <stdio.h>

# include <stdlib.h>

# include <unistd.h>

# include <signal.h>

# include <errno.h>

# include <fcntl.h>

# include <sys/types.h>

# include <sys/stat.h>

# include <sys/wait.h>

# include <readline/readline.h>

# include <readline/history.h>

# include "libft.h"

/* ============================== COLORS ================================ */

# define COLOR_GOLD     "\x1b[38;5;220m"

# define COLOR_BLACK    "\x1b[30m"

# define COLOR_RED      "\x1b[31m"

# define COLOR_GREEN    "\x1b[32m"

# define COLOR_YELLOW   "\x1b[33m"

# define COLOR_BLUE     "\x1b[34m"

# define COLOR_MAGENTA  "\x1b[35m"

# define COLOR_CYAN     "\x1b[36m"

# define COLOR_WHITE    "\x1b[37m"

# define COLOR_RESET    "\x1b[0m"

#ifndef MINISHELL_BANNER
# define MINISHELL_BANNER ""
#endif

/* ============================== GLOBALS =============================== */

extern volatile sig_atomic_t	g_signal;

/* ============================== TYPES ================================= */

typedef enum e_type
{
	T_STRING,
	T_CMD,
	T_PIPE,
	T_REDIR_OUT,
	T_REDIR_IN,
	T_REDIR_APPEND,
	T_HEREDOC
}	t_type;

typedef struct s_token
{
	char			*data;
	t_type			type_tok;
	int				quoted;
	struct s_token	*next;
}	t_token;

typedef struct s_heredoc
{
	char				*delimiter;
	int					expand;
	int					fd;
	struct s_heredoc	*next;
}	t_heredoc;

typedef struct s_redir
{
	t_type			type;// IN OUT APPEND HEREDOC
	char			*file;// filename or delimiter
	int				fd;// used for ready heredoc
	int				expand;// just for heredoc
	struct s_redir	*next;
}	t_redir;

typedef struct s_ast
{
	t_type			type;
	char			**args;
	t_redir			*redirs;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

typedef struct s_shell
{
	char	*str;
	char	**envp;
	int		exit_status;
	t_token	*tokens;
	t_ast	*ast;
	int		in_pipeline;
}	t_shell;

typedef struct s_pipe
{
	t_ast			*node;
	int				input_fd;
	int				pipe_fd[2];
	pid_t			pid;
	struct s_pipe	*next;
}	t_pipe;

/* ============================== UTILS ================================= */

char	**envp_dup(char **env);
void	free_string_array(char **str);
char	*ft_strndup(const char *s, size_t n);
int		env_length(char **env);
int		get_env_index(char **env, char *name);
char	*get_env_value(const char *var, char **env);
void	free_ast(t_ast *node);

/* ============================== LEXER ================================= */

t_token	*lexer(const char *input, char **env, int exit_status);
void	free_tokens(t_token *token);

/* ============================== PARSER ================================ */

t_ast	*parser(t_shell *sh);
t_ast	*init_ast(void);
t_redir	*init_redir(t_type type);
t_ast	*parse_pipe(t_token **tokens);
t_ast	*parse_redirection(t_token **token);
t_ast	*parse_command_segment(t_token **token);

char	**parse_command(t_token **tokens);
char	**parse_args(t_token **token);

void	add_redir(t_redir **list, t_redir *new_redir);
t_redir	*create_redir(char *op, char *file);

int		check_tokens(t_token *token);
int		is_operator(t_type type);
int		is_redirection(t_type type);

/* ============================== EXEC ================================== */

int		execute_ast(t_ast *node, char ***envp, int last_status);
int		execute_cmd(t_ast *node, char ***envp);
int		execute_pipe(t_ast *node, char ***envp);
void	execute_redirection(t_ast *node, char **envp);
int		apply_redirections(t_redir *redir);

void	execute(char **args, char ***envp);
char	*get_path(char *cmd, char **env);

t_pipe	*build_pipe_list(t_ast *ast);
t_pipe	*new_pipe(t_ast *node);
void	free_pipe_list(t_pipe *head);

int		redirect_fd(int fd, int target);
int		return_status(pid_t pid, int *sig);
void	sig_write(int sig);
void	fatal(char *arg, char *msg, int exit_code);

/* ============================== HEREDOC =============================== */

int		read_heredoc(t_redir heredoc, char **envp, int last_status);
int		prepare_heredocs(t_ast *ast, char **envp, int last_status);

/* ============================== SIGNALS =============================== */

void	set_signals_prompt(void);
void	set_signals_parent_exec(void);
void	set_signals_child(void);
void	sigint_handler(int sig);

void	set_signals_heredoc(void);

/* ============================== BUILTINS ============================== */

int		is_builtin(char *str);
int		builtin(char **args, char ***envp);

int		ft_cd(char **args, char ***envp);
int		ft_echo(char **args);
int		ft_pwd(void);
int		ft_exit(char **args);
int		ft_unset(char **args, char ***envp);
int		ft_export(char **args, char ***envp);
int		print_export_sorted(char **envp);
int		ft_env(char **args, char **envp);

void	handle_export_arg(char ***envp, char *arg);
int		is_valid_identifier(char *s);

/* ============================== SHELL ================================ */

t_shell	init_shell(char **envp);
void	free_shell(t_shell *sh);

/* ===================================================================== */

#endif
