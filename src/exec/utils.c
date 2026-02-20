/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 17:00:00 by jmarques          #+#    #+#             */
/*   Updated: 2025/12/10 17:00:02 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_write(int sig)
{
	if (sig == SIGINT)
		write(1, "\n", 1);
	else if (sig == SIGQUIT)
		write(1, "Quit (core dumped)\n", 19);
}

int	return_status(pid_t pid, int *sig)
{
	int	status;
	int	last_status;

	last_status = 0;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		*sig = WTERMSIG(status);
		last_status = 128 + WTERMSIG(status);
	}
	return (last_status);
}

void	fatal(char *arg, char *msg, int exit_code)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(msg, 2);
	exit(exit_code);
}
