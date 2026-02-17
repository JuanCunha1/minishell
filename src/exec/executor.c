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

int	return_status(pid_t pid)
{
	int	status;
	int	last_status;

	last_status = 1;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		last_status = 128 + WTERMSIG(status);
	return (last_status);
}

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
	return (return_status(pid));
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
