/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 16:22:26 by jmarques          #+#    #+#             */
/*   Updated: 2026/01/08 16:22:28 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>

void	handle_signal_heredoc(int sig)
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

static void	handle_signal(int sig)
{
	if (sig == SIGQUIT)
		return ;
	g_signal = 130;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	set_signals_parent_exec(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	set_signals_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	set_signals_prompt(void)
{
	g_signal = 0;
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
}
