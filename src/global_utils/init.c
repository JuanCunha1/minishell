/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 12:28:54 by jmarques          #+#    #+#             */
/*   Updated: 2026/02/17 12:29:01 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell	init_shell(char **envp)
{
	t_shell	sh;

	sh.exit_status = 0;
	sh.in_pipeline = 0;
	sh.tokens = NULL;
	sh.ast = NULL;
	sh.str = NULL;
	sh.envp = envp_dup(envp);
	return (sh);
}

t_ast	*init_ast(void)
{
	t_ast	*ast;

	ast = malloc(sizeof(t_ast));
	if (!ast)
		return (NULL);
	ast->type = T_STRING;
	ast->left = NULL;
	ast->right = NULL;
	ast->redirs = NULL;
	ast->heredocs = NULL;
	ast->args = NULL;
	return (ast);
}

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
