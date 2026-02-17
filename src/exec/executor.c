/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 14:19:54 by jmarques          #+#    #+#             */
/*   Updated: 2025/12/02 10:34:06 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
 * execute a simple command
void	execute_simple(t_ast *node, t_shell *sh)
{
	char	*path;

	if (!node->args || !node->args[0])
		return ;
	if (is_builtin(node->args[0]))
	{
		builtin(node, sh, node->args[0]);
		if (sh->in_pipeline)
			exit(sh->exit_status);
		return ;
	}
	path = get_path(node->args[0], sh->envp);
	if (!path)
	{
		ft_putstr_fd(node->args[0], 2);
		ft_putendl_fd(": command not found", 2);
		exit(127);
	}
	set_signals_child();
	execve(path, node->args, sh->envp);
	free(path);
	perror(node->args[0]);
	exit(1);
}

void	exec_node_no_fork(t_ast *node, t_shell *sh)
{
	if (node->type == T_PIPE)
		execute_pipe(node, sh);
	else if (node->type == T_STRING)
		execute_simple(node, sh);
	else if (node->type == T_REDIR_IN
		|| node->type == T_REDIR_OUT
		|| node->type == T_REDIR_APPEND)
		execute_redirection(node, sh);
	exit(sh->exit_status);
}

void	exec_node(t_ast *node, t_shell *sh)
{
	pid_t	pid;
	int		status;

	if (sh->in_pipeline)
	{
		exec_node_no_fork(node, sh);
		exit(sh->exit_status);
	}
	set_signals_exec();
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		sh->exit_status = 1;
		return ;
	}
	if (pid == 0)
	{
		exec_node_no_fork(node, sh);
		exit(sh->exit_status);
	}
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		write(1,"\n", 1);
	}
	if (WIFEXITED(status))
		sh->exit_status = WEXITSTATUS(status);
	else
		sh->exit_status = 1;
}*/
/*
void	execute_ast(t_ast *ast, t_shell *sh)
{
	if (!sh || !ast)
		return ;
	if (ast->type == T_HEREDOC)
	{
		if (process_heredocs(ast, sh) == -1)
		{
			ft_putendl_fd("Error processing heredoc", 2);
			sh->exit_status = 1;
			return ;
		}
	}
	if (ast->type == T_STRING && is_builtin(ast->args[0])
		&& !sh->in_pipeline)
		builtin(ast, sh, ast->args[0]);
	else
		exec_node(ast, sh);
}
*/

int	exec_builtin_parent(t_ast *node, char ***envp)
{
	int	saved_stdin;
	int	saved_stdout;
	int	return_value;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	apply_redirections(node->redirs);
	return_value = builtin(node->args, envp);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (return_value);
}

void	execute(t_ast *node, char ***envp)
{
	char	*path;

	apply_redirections(node->redirs);
	if (is_builtin(node->args[0]))
		exit(builtin(node->args, envp));
	path = get_path(node->args[0], *envp);
	if (!path)
	{
		ft_putstr_fd(node->args[0], 2);
		ft_putendl_fd(": command not found", 2);
		exit(127);
	}
	execve(path, node->args, *envp);
	perror(node->args[0]);
	free(path);
	exit(127);
}

int	execute_cmd(t_ast *node, char ***envp, int in_pipe)
{
	pid_t	pid;
	int		status;

	if (!node->args || !node->args[0])
		return (0);
	if (is_builtin(node->args[0]) && !in_pipe)
		return (exec_builtin_parent(node, envp));
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		set_signals_child();
		execute(node, envp);
		exit(127);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	execute_ast(t_ast *node, char ***envp, int in_pipe)
{
	if (!node)
		return (1);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (node->type == T_PIPE)
		return (execute_pipe(node, envp));
	return (execute_cmd(node, envp, in_pipe));
}
