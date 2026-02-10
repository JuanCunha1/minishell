/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 13:29:54 by jmarques          #+#    #+#             */
/*   Updated: 2025/12/02 13:29:56 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	redirect_fd(int fd, int target)
{
	if (dup2(fd, target) < 0)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	open_redir_file(t_ast *node)
{
	int	fd;

	fd = -1;
	if (node->type == T_REDIR_IN)
		fd = open(node->file, O_RDONLY);
	else if (node->type == T_REDIR_OUT)
		fd = open(node->file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	else if (node->type == T_REDIR_APPEND)
		fd = open(node->file, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (fd < 0)
		perror(node->file);
	return (fd);
}

int	apply_redirections(t_ast *redir)
{
	int	fd;

	while (redir && redir->type >= T_REDIR_OUT && redir->type <= T_HEREDOC)
	{
		fd = open_redir_file(redir);
		if (fd < 0)
			return (-1);
		if (redir->type == T_REDIR_IN || redir->type == T_HEREDOC)
		{
			if (redirect_fd(fd, STDIN_FILENO) < 0)
				return (-1);
		}
		else
		{
			if (redirect_fd(fd, STDOUT_FILENO) < 0)
				return (-1);
		}
		redir = redir->left;
	}
	return (0);
}

void	execute_redirection(t_ast *node, t_shell *sh)
{
	if (!node)
		return ;
	if (apply_redirections(node) == -1)
		exit(1);
	execute_ast(node->left, sh);
	exit(sh->exit_status);
}
