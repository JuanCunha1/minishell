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

static char	*resolve_path(char **args, char ***envp, int *alloc)
{
	char	*path;

	path = args[0];
	*alloc = 0;
	if (!ft_strchr(args[0], '/'))
	{
		path = get_path(args[0], *envp);
		if (!path)
			fatal(args[0], ": command not found", 127);
		*alloc = 1;
	}
	return (path);
}

int	exec_builtin_parent(t_ast *node, char ***envp)
{
	int	saved_stdin;
	int	saved_stdout;
	int	return_value;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (apply_redirections(node->redirs))
		return (1);
	return_value = builtin(node->args, envp);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (return_value);
}

void	execute(char **args, char ***envp)
{
	char	*path;
	int		err;
	int		alloc;

	if (!args[0] || args[0][0] == '\0')
		fatal("", ": command not found", 127);
	if (is_builtin(args[0]))
		exit(builtin(args, envp));
	path = resolve_path(args, envp, &alloc);
	execve(path, args, *envp);
	err = errno;
	perror(path);
	if (alloc)
		free(path);
	if (err == EACCES || err == EISDIR)
		exit(126);
	exit(127);
}

int	execute_cmd(t_ast *node, char ***envp)
{
	pid_t	pid;
	int		status;
	int		sig;

	if (!node->args || !node->args[0])
		return (0);
	if (is_builtin(node->args[0]))
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
		apply_redirections(node->redirs);
		execute(node->args, envp);
		exit(127);
	}
	status = return_status(pid, &sig);
	sig_write(sig);
	return (status);
}

int	execute_ast(t_ast *node, char ***envp)
{
	if (!node)
		return (1);
	if (node->type == T_PIPE)
		return (execute_pipe(node, envp));
	return (execute_cmd(node, envp));
}
