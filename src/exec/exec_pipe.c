/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 13:29:43 by jmarques          #+#    #+#             */
/*   Updated: 2025/12/02 13:29:47 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/types.h>

static void	child_setup(int input_fd, int pipe_out)
{
	if (input_fd != STDIN_FILENO)
	{
		if (dup2(input_fd, STDIN_FILENO) < 0)
			fatal("dup2 input");
		close(input_fd);
	}
	if (pipe_out != STDOUT_FILENO)
	{
		if (dup2(pipe_out, STDOUT_FILENO) < 0)
			fatal("dup2 output");
		close(pipe_out);
	}
}

/*
 * creates a new child process and sets
 * up the appropriate file descriptors for input/output redirection.
 */
static void	spawn_child(t_pipe *pip, int pipe_out)
{
	set_signals_exec();
	pip->pid = fork();
	if (pip->pid < 0)
		fatal("fork");
	if (pip->pid == 0)
	{
		child_setup(pip->input_fd, pipe_out);
		free(pip->pids);
		execute_ast(pip->node, pip->sh);
		exit(pip->sh->exit_status);
	}
	if (pipe_out != STDOUT_FILENO)
		close(pipe_out);
}

/*
 * Ensures that the pids array has enough space
 * to store the process IDs (pids) of all child processes.
 */
static void	ensure_capacity(t_pipe *pip)
{
	if (pip->count >= pip->capacity)
	{
		pip->capacity += 10;
		pip->pids = realloc(pip->pids, sizeof(pid_t) * pip->capacity);
		if (!pip->pids)
			fatal("realloc");
	}
}

/*
 * handles the creation of a pipeline of processes,
 * ensuring each child is connected properly via pipes.
 */
void	pipe_nodes(t_pipe *pip)
{
	t_ast	*current;

	current = pip->node;
	while (current && current->type == T_PIPE)
	{
		if (pipe(pip->pipe_fd) < 0)
			fatal("pipe");
		pip->node = current->left;
		spawn_child(pip, pip->pipe_fd[1]);
		pip->pids[pip->count++] = pip->pid;
		if (pip->input_fd != STDIN_FILENO)
			close(pip->input_fd);
		pip->input_fd = pip->pipe_fd[0];
		current = current->right;
		ensure_capacity(pip);
	}
	pip->node = current;
}

/*
 * manages the execution of the entire pipeline,
 * from initializing the pipeline structure to handling
 * process execution and waiting for all child processes.
 */
void	execute_pipe(t_ast *node, t_shell *sh)
{
	t_pipe	pip;
	int		status;
	int		last_index;

	sh->in_pipeline = 1;
	pip = init_pipe(node, sh);
	pipe_nodes(&pip);
	spawn_child(&pip, STDOUT_FILENO);
	pip.pids[pip.count++] = pip.pid;
	if (pip.input_fd != STDIN_FILENO)
		close(pip.input_fd);
	last_index = pip.count - 1;
	waitpid(pip.pids[last_index], &status, 0);
	while (--last_index >= 0)
		waitpid(pip.pids[last_index], NULL, 0);
	free(pip.pids);
	sh->in_pipeline = 0;
	if (WIFEXITED(status))
		sh->exit_status = WEXITSTATUS(status);
	else
		sh->exit_status = 1;
	free_ast(sh->ast);
}
