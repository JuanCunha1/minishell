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
		ft_putstr_fd("minishell: ", 2);
		perror(redir->file);
		return (fd);
	}
	return (fd);
}

int	apply_redirections(t_redir *redir)
{
	int	fd;
	int	target;

	while (redir)
	{
		fd = open_redir_file(redir);
		if (fd < 0)
			return (1);
		if (redir->type == T_REDIR_IN)
			target = STDIN_FILENO;
		else
			target = STDOUT_FILENO;
		if (redirect_fd(fd, target) < 0)
		{
			if (fd != target)
				close(fd);
			return (1);
		}
		redir = redir->next;
	}
	return (0);
}
