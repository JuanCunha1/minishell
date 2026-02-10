/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 12:39:10 by jmarques          #+#    #+#             */
/*   Updated: 2025/11/26 12:39:16 by jmarques         ###   ########.fr       */
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

/*----STRUCTS----
 * ESTE ES UN ENUM PARA SELECCIONAR EL TIPO DE TOKEN QUE ES PARA
 * EVITAR TENER MUCHOS IF COMPARANDO QUE ES.
 * FUNCION EN PROGRESO.
 * */
/*
typedef enum e_node_type
{
	CMD,
	PIPE,
	REDIR_OUT,
	REDIR_IN,
	REDIR_APPEND,
	HEREDOC
}	t_node_type;
*/
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

typedef struct s_flags
{
	int			flag_simple_quot;
	int			flag_double_quot;
	int			flag_heredoc;
}				t_flags;

/*ESTA ES LA STRUCT PARA GUARDAR LOS TOKENS*/
typedef struct s_token
{
	char			*data;
	t_type			type_tok;
	struct s_token	*next;
}	t_token;

typedef struct s_ast
{
	t_type			type;
	char			**args;
	char			*file;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

/*STRUCT BASE, TIPICA STRUCT PARA PASAR VARIABLES GENERALES*/
typedef struct s_shell
{
	char	*str;
	char	**envp;
	int		exit_status; //probable futuro manejo de exit
	t_token	*tokens;
	t_ast	*ast;
	t_flags	flags;
	int		in_pipeline;
}	t_shell;

typedef struct s_pipe
{
	t_ast	*node;
	t_shell	*sh;
	int		input_fd;
	int		pipe_fd[2];
	pid_t	pid;
	pid_t	*pids;
	size_t	count;
	size_t	capacity;
}	t_pipe;

/*----FUNCTIONS----*/

void	free2d(char **arr);
char	**envp_dup(char **ae);
void	clear_and_leave(t_shell *base, char **args);

/*Tokenizer & Parser*/
int		is_heredoc(char *str, t_flags *flags);
int		lexer(t_shell *sh);

char	**ft_split(char const *s, char c);
char	*asignar_palabra(const char *s, int len);
void	print_list(t_token **l_tokens);
void	init_list(t_token **tokens, char *str, t_flags *flags);
int		ft_count_word(char const *s, char c);
int		contador_letras_comis(char const *s, char c);
int		getype(char *str, t_flags *flags);
int		is_simple_quoted(char *str, t_flags *flags);
int		is_double_quoted(char *str, t_flags *flags);
int		is_pipe(char *str, t_flags *flags);
int		is_or_operator(char *str, t_flags *flags);
void	init_flags(t_flags *flags);
void	init_base(char **ae, t_shell *base);

/*Parser*/
int		parser(t_shell *sh);
int		create_ast(t_token *token, t_ast *ast);
int		redirection(t_token **token, t_ast *ast);
int		parse_pipe(t_token **token, t_ast *ast);
//void	init_ast(t_ast *ast);

/* execute*/
void	execute_ast(t_ast *ast, t_shell *sh);
void	ft_free_tab(char **tab);
char	*get_path(char *cmd, char **env);
void	exec_node(t_ast *node, t_shell *sh);
void	execute_pipe(t_ast *node, t_shell *sh);
int		execute_pipe_recursive(t_ast *node, t_shell *sh, int input_fd);
void	exec_node_no_fork(t_ast *node, t_shell *sh);
void	free_ast(t_ast *node);
void	fatal(const char *msg);
void	execute_redirection(t_ast *node, t_shell *sh);
t_pipe	init_pipe(t_ast *node, t_shell *sh);

void	free_tokens(t_token *token);

void	free_shell(t_shell *sh);

/* signal */
void	set_signals_prompt(void);
void	set_signals_exec(void);
void	set_signals_child(void);
/* Builtins*/

int		is_builtin(char *str);
void	builtin(t_ast *ast, t_shell *sh, char *str);

void	ft_cd(t_ast *ast, t_shell *shell);

void	ft_echo(char *args[], t_shell *shell);

void	ft_pwd(t_shell *shell);

void	ft_exit(t_shell *shell);

void	ft_unset(t_shell *shell);

void	ft_export(t_shell *sh);
void	handle_export_arg(t_shell *shell, char *name, int has_equal, char *arg);

void	ft_env(t_shell *sh);


t_shell	init_shell(char **envp);
t_pipe	init_pipe(t_ast *node, t_shell *sh);
t_ast	*init_ast(void);
void	free_string_array(char **str);
#endif
