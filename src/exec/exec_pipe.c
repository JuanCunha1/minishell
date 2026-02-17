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

void	pipe_pid(t_pipe *p, char ***envp)
{
	if (p->input_fd != STDIN_FILENO
		&& redirect_fd(p->input_fd, STDIN_FILENO) < 0)
		exit(1);
	if (p->pipe_fd[1] != -1
		&& redirect_fd(p->pipe_fd[1], STDOUT_FILENO) < 0)
		exit(1);
	if (p->pipe_fd[0] != -1)
		close(p->pipe_fd[0]);
	apply_redirections(p->node->redirs);
	execute(p->node->args, envp);
	exit(1);
}

static int	fork_pipe_cmd(t_pipe *p, char ***envp)
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
		pipe_pid(p, envp);
	}
	return (p->pid);
}

static void	execute_pipe_list(t_pipe *head, char ***envp)
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
				return ;
			}
			p->next->input_fd = p->pipe_fd[0];
		}
		if (fork_pipe_cmd(p, envp) < 0)
			return ;
		if (p->input_fd != STDIN_FILENO)
			close(p->input_fd);
		if (p->pipe_fd[1] != -1)
			close(p->pipe_fd[1]);
		p = p->next;
	}
	return ;
}

t_pipe	*build_pipe_list(t_ast *node)
{
	t_pipe	*head;
	t_pipe	*current;
	t_pipe	*new_p;

	if (!node)
		return (NULL);
	if (node->type == T_PIPE)
	{
		head = build_pipe_list(node->left);
		new_p = new_pipe(node->right);
		if (!new_p)
		{
			free_pipe_list(head);
			return (NULL);
		}
		if (!head)
			return (new_p);
		current = head;
		while (current->next)
			current = current->next;
		current->next = new_p;
		return (head);
	}
	return (new_pipe(node));
}

int	execute_pipe(t_ast *node, char ***envp)
{
	t_pipe	*pipes;
	int		last_status;
	t_pipe	*tmp;
	int		sig;

	pipes = build_pipe_list(node);
	if (!pipes)
		return (1);
	execute_pipe_list(pipes, envp);
	last_status = 0;
	tmp = pipes;
	while (tmp)
	{
		last_status = return_status(tmp->pid, &sig);
		tmp = tmp->next;
	}
	sig_write(sig);
	free_pipe_list(pipes);
	return (last_status);
}
