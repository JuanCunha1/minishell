/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 16:22:26 by jmarques          #+#    #+#             */
/*   Updated: 2026/02/24 16:22:28 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_signal_heredoc(int sig)
{
	if (sig == SIGQUIT)
		return ;
	g_signal = 130;
	write(1, "\n", 1);
	exit(130);
}

void	set_signals_heredoc(void)
{
	g_signal = 0;
	signal(SIGINT, handle_signal_heredoc);
	signal(SIGQUIT, SIG_IGN);
}
