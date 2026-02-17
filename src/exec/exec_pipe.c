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

t_pipe	*new_pipe(t_ast *node)
{
	t_pipe	*p;

	p = malloc(sizeof(t_pipe));
	if (!p)
		return (NULL);
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
	if (fd != target)
	{
		if (dup2(fd, target) < 0)
		{
			perror("dup2");
			return (-1);
		}
		close(fd);
	}
	return (0);
}

int	pipe_pid(t_pipe *p, char ***envp)
{
	if (p->input_fd != STDIN_FILENO
		&& redirect_fd(p->input_fd, STDIN_FILENO) < 0)
		return (-1);
	if (p->pipe_fd[1] != -1
		&& redirect_fd(p->pipe_fd[1], STDOUT_FILENO) < 0)
		return (-1);
	if (p->pipe_fd[0] != -1)
		close(p->pipe_fd[0]);
	return (execute_cmd(p->node, envp, 1));
}

int	fork_pipe_cmd(t_pipe *p, char ***envp)
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

int	execute_pipe_list(t_pipe *head, char ***envp)
{
	t_pipe	*p;

	p = head;
	while (p)
	{
		if (p->next)
		{
			if (pipe(p->pipe_fd) < 0)
			{
				perror("pipe");
				return (1);
			}
			p->next->input_fd = p->pipe_fd[0];
		}
		fork_pipe_cmd(p, envp);
		if (p->input_fd != STDIN_FILENO)
			close(p->input_fd);
		if (p->pipe_fd[1] != -1)
			close(p->pipe_fd[1]);
		p = p->next;
	}
	return (return_status(head));
}

t_pipe	*build_pipe_list(t_ast *node)
{
	t_pipe	*head;
	t_pipe	*current;
	t_pipe	*new;

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

int	execute_pipe(t_ast *node, char ***envp)
{
	t_pipe	*pipes;
	int		last_status;
	t_pipe	*tmp;
	t_pipe	*next;

	pipes = build_pipe_list(node);
	if (!pipes)
		return (-1);
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
