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
/*
static void	child_setup(int input_fd, int pipe_out)
{
	if (input_fd != STDIN_FILENO)
	{
		if (dup2(input_fd, STDIN_FILENO) < 0)
			fatal("dup2 input", 1);
		close(input_fd);
	}
	if (pipe_out != STDOUT_FILENO)
	{
		if (dup2(pipe_out, STDOUT_FILENO) < 0)
			fatal("dup2 output", 1);
		close(pipe_out);
	}
}*/

/*
 * creates a new child process and sets
 * up the appropriate file descriptors for input/output redirection.
 
static void	spawn_child(t_pipe *pip, int pipe_out)
{
	set_signals_exec();
	pip->pid = fork();
	if (pip->pid < 0)
		fatal("fork", 1);
	if (pip->pid == 0)
	{
		child_setup(pip->input_fd, pipe_out);
		free(pip->pids);
		execute_ast(pip->node, pip->sh);
		exit(pip->sh->exit_status);
	}
	if (pipe_out != STDOUT_FILENO)
		close(pipe_out);
}*/

/*
 * Ensures that the pids array has enough space
 * to store the process IDs (pids) of all child processes.
 
static void	ensure_capacity(t_pipe *pip)
{
	if (pip->count >= pip->capacity)
	{
		pip->capacity += 10;
		pip->pids = realloc(pip->pids, sizeof(pid_t) * pip->capacity);
		if (!pip->pids)
			fatal("realloc", 1);
	}
}*/

/*
 * handles the creation of a pipeline of processes,
 * ensuring each child is connected properly via pipes.
 
void	pipe_nodes(t_pipe *pip)
{
	t_ast	*current;

	current = pip->node;
	while (current && current->type == T_PIPE)
	{
		if (pipe(pip->pipe_fd) < 0)
			fatal("pipe", 1);
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
}*/

/*
 * manages the execution of the entire pipeline,
 * from initializing the pipeline structure to handling
 * process execution and waiting for all child processes.
 */
 /*
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
}*/
t_pipe	*new_pipe(t_ast *node)
{
	t_pipe	*p;

	p = malloc(sizeof(t_pipe));
	if (!p)
		return NULL;

	p->node = node;
	p->input_fd = STDIN_FILENO;
	p->pipe_fd[0] = -1;
	p->pipe_fd[1] = -1;
	p->pid = -1;
	p->next = NULL;
	return (p);
}

int	redirect_fd(int fd, int target)
{
	if (dup2(fd, target) < 0)
	{
		perror("dup2");
		return -1;
	}
	close(fd);
	return (0);
}

int pipe_pid(t_pipe *p, char **envp)
{
	if (p->input_fd != STDIN_FILENO)
	{
		if (redirect_fd(p->input_fd, STDIN_FILENO) < 0)
			return (-1);
	}
	if (p->pipe_fd[1] != -1)
	{
		if (redirect_fd(p->pipe_fd[1], STDOUT_FILENO) < 0)
			return (-1);
	}
	if (p->pipe_fd[0] != -1)
		close(p->pipe_fd[0]);
	execute_cmd(p->node, envp, 1);
	return (127);
}
int fork_pipe_cmd(t_pipe *p, char **envp)
{
	if (!p)
		return (-1);
	if (p->pid != -1)
		return (p->pid);
	p->pid = fork();
	if (p->pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (p->pid == 0)
	{
		set_signals_child();
		exit(pipe_pid(p, envp));
	}
	return (p->pid);
}

int	return_status(t_pipe *p)
{
	int	status;
	int	last_status;

	last_status = 0;
	while (p)
	{
		waitpid(p->pid, &status, 0);
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			last_status = 128 + WTERMSIG(status);
		p = p->next;
	}
	return (last_status);
}

int execute_pipe_list(t_pipe *head, char **envp)
{
	t_pipe *p;

	p = head;
	while (p)
	{
		if (p->next)
		{
			if (pipe(p->pipe_fd) < 0)
			{
				perror("pipe");
				return 1;
			}
			p->next->input_fd = p->pipe_fd[0];
		}
		fork_pipe_cmd(p, envp);
		if (p->pipe_fd[1] != -1)
			close(p->pipe_fd[1]);
		p = p->next;
	}
	return (return_status(head));
}
/*
pid_t	pipe_fork(t_ast *node, char **envp, int target, int fd[2])
{
	pid_t	pid;
	int		is_output;

	is_output = (target == STDOUT_FILENO);
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		set_signals_child();
		if(redirect_fd(fd, target, is_output) < 0)
			exit(1);
		execute_cmd(node, envp, 1);
		exit(127);
	}
	return (pid);
}*/
t_pipe *build_pipe_list(t_ast *node)
{
	t_pipe *head;
	t_pipe *current;
	t_pipe *new;

	if (!node)
		return (NULL);
	if (node->type == T_PIPE)
	{
		head = build_pipe_list(node->left);
		new = new_pipe(node->right);
		if (!new)
			return (NULL);
		if (!head)
			head = new;
		else
		{
			current = head;
			while (current->next)
				current = current->next;
			current->next = new;
		}
	}
	else
		head = new_pipe(node);
	return (head);
}
int	execute_pipe(t_ast *node, char **envp)
{
	t_pipe	*pipes;
	int		last_status;
	t_pipe	*tmp;
	t_pipe	*next;

	pipes = build_pipe_list(node);
	if (!pipes)
		return (1);
	last_status = execute_pipe_list(pipes, envp);
	tmp = pipes;
	while (tmp)
	{
		next = tmp->next;
		free(tmp);
		tmp = next;
	}
	return (last_status);
}