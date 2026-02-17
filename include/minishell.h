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

/*----LIBRERY----*/
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>

/* ─── COLORES DE TEXTO ────────────────────────────────*/
# define COLOR_GOLD "\x1b[38;5;220m"
# define COLOR_BLACK   "\x1b[30m"
# define COLOR_RESET "\x1b[0m"
# define COLOR_RED     "\x1b[31m"
# define COLOR_GREEN   "\x1b[32m"
# define COLOR_YELLOW  "\x1b[33m"
# define COLOR_BLUE    "\x1b[34m"
# define COLOR_MAGENTA "\x1b[35m"
# define COLOR_CYAN    "\x1b[36m"
# define COLOR_WHITE   "\x1b[37m"
# define INITIAL_PID_CAPACITY 10

# define MINISHELL_BANNER COLOR_GOLD "\n\
                 \\__|          \\__| $$$$$$\\  $$ |                $$ |$$ |\n\
   $$$$$$\\$$$$\\  $$\\ $$$$$$$\\  $$\\ $$  __$$\\ $$$$$$$\\   $$$$$$\\  $$ |$$ |\n\
   $$  _$$  _$$\\ $$ |$$  __$$\\ $$ |$$ /  \\__|$$  __$$\\ $$  __$$\\ $$ |$$ |\n\
   $$ / $$ / $$ |$$ |$$ |  $$ |$$ |\\$$$$$$\\  $$ |  $$ |$$$$$$$$ |$$ |$$ |\n\
   $$ | $$ | $$ |$$ |$$ |  $$ |$$ | \\____$$\\ $$ |  $$ |$$   ____|$$ |$$ |\n\
   $$ | $$ | $$ |$$ |$$ |  $$ |$$ |$$\\   $$ |$$ |  $$ |\\$$$$$$$\\ $$ |$$ |\n\
   \\__| \\__| \\__|\\__|\\__|  \\__|\\__|\\$$$$$$  |\\__|  \\__| \\_______|\\__|\\__|\n\
                                    \\______/                             \n\
\n" COLOR_RESET

extern volatile sig_atomic_t g_signal;

/*----STRUCTS----*/

typedef enum e_type
{
	T_STRING, /*---0---*/
	T_SIMPLE_QUOTED, /*---1---*/
	T_DOUBLE_QUOTED, /*---2---*/
	T_APPEND, /*---3---*/
	T_COMMAND, /*---4---*/
	T_PIPE, /*---6---*/
	T_FLOW_OPERATOR, /*---7---*/
	T_REDIR_OUT, /*---8---*/
	T_REDIR_IN, /*---9---*/
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
	t_type			type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

typedef struct s_ast
{
	t_type			type;
	char			**args;
	t_redir			*redirs;
	t_heredoc		*heredocs;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

typedef struct s_shell
{
	char		*str;
	char		**envp;
	int			exit_status;
	t_token		*tokens;
	t_ast		*ast;
	int			in_pipeline;
}	t_shell;

typedef struct s_pipe
{
	t_ast			*node;
	int				input_fd;
	int				pipe_fd[2];
	pid_t			pid;
	struct s_pipe	*next;
}	t_pipe;

/*----FUNCTIONS----*/

void	free2d(char **arr);
char	**envp_dup(char **ae);
void	clear_and_leave(t_shell *base, char **args);
char	*ft_strndup(const char *s, size_t n);

t_token	*lexer(const char *input, char **env, int exit_status);

/*Parser*/
t_ast	*parser(t_shell *sh);
int		create_ast(t_token *token, t_ast *ast);

/* execute*/
void	ft_free_tab(char **tab);
char	*get_path(char *cmd, char **env);
void	exec_node(t_ast *node, t_shell *sh);
int		execute_pipe_recursive(t_ast *node, t_shell *sh, int input_fd);
void	exec_node_no_fork(t_ast *node, t_shell *sh);
void	free_ast(t_ast *node);
void	fatal(const char *msg, int exit_code);
t_pipe	init_pipe(t_ast *node, t_shell *sh);

void	free_tokens(t_token *token);

void	free_shell(t_shell *sh);

/* signal */
void	set_signals_prompt(void);
void	set_signals_parent_exec(void);
void	set_signals_child(void);
/* Builtins*/

int		is_builtin(char *str);
int		builtin(char **args, char ***envp);

int		ft_cd(char **args, char ***envp);

int		ft_echo(char **args);

int		ft_pwd(void);

int		ft_exit(char **args);

int		ft_unset(char **args, char ***envp);

int		ft_export(char **args, char ***envp);
void	handle_export_arg(char ***envp, char *arg);

int		ft_env(char **args, char **envp);

char	*get_env_value(const char *var, char **env);

t_shell	init_shell(char **envp);
t_pipe	init_pipe(t_ast *node, t_shell *sh);
t_ast	*init_ast(void);
void	free_string_array(char **str);

int		process_heredocs(t_ast *ast, t_shell *sh);
int		read_heredoc(t_heredoc *hd, t_shell *sh);

t_ast	*parse_pipe(t_token **tokens);
char	**parse_command(t_token **tokens);
void	add_redir(t_redir **list, t_redir *new_redir);
t_redir	*create_redir(char *op, char *file);
int		is_redirection(t_type type);
t_ast	*parse_redirection(t_token **token);
int		execute_ast(t_ast *node, char ***envp);
void	execute_redirection(t_ast *node, char **envp);
int		execute_pipe(t_ast *node, char ***envp);
int		apply_redirections(t_redir *redir);
int		execute_cmd(t_ast *node, char ***envp);

void	set_signals_prompt(void);
void	sigint_handler(int sig);
t_ast	*parse_command_segment(t_token **token);
void	execute(t_ast *node, char ***envp);
t_pipe	*build_pipe_list(t_ast *ast);
int		check_tokens(t_token *token);
int		is_operator(t_type type);
char	**parse_args(t_token **token);
int		env_length(char **env);
int		redirect_fd(int fd, int target);
void	free_pipe_list(t_pipe *head);
int		return_status(pid_t pid);
t_pipe	*new_pipe(t_ast *node);
#endif
