/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 12:29:07 by jmarques          #+#    #+#             */
/*   Updated: 2026/02/17 12:29:08 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_shell(t_shell *sh)
{
	if (!sh)
		return ;
	if (sh->str)
	{
		free(sh->str);
		sh->str = NULL;
	}
	if (sh->tokens)
	{
		free_tokens(sh->tokens);
		sh->tokens = NULL;
	}
	if (sh->ast)
	{
		free_ast(sh->ast);
		sh->ast = NULL;
	}
}

int	cleanup(t_shell *sh)
{
	free_shell(sh);
	if (sh->exit_status >= 256)
	{
		free_string_array(sh->envp);
		rl_clear_history();
		exit(sh->exit_status - 256);
	}
	return (1);
}
