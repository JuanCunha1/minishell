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

static int	open_redir_file(t_redir *redir)
{
	int	fd;

	fd = -1;
	if (redir->type == T_REDIR_IN)
		fd = open(redir->file, O_RDONLY);
	else if (redir->type == T_REDIR_OUT)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == T_REDIR_APPEND)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(redir->file);
		exit(1);
	}
	return (fd);
}

static void	redirect_dup(int fd, int target)
{
	if (dup2(fd, target) < 0)
	{
		perror("dup2");
		close(fd);
		exit(1);
	}
}

int	apply_redirections(t_redir *redir)
{
	int	fd;

	while (redir)
	{
		fd = open_redir_file(redir);
		if (redir->type == T_REDIR_IN)
			redirect_dup(fd, STDIN_FILENO);
		else
			redirect_dup(fd, STDOUT_FILENO);
		close(fd);
		redir = redir->next;
	}
	return (0);
}
