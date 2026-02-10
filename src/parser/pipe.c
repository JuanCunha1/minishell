/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 15:44:34 by jmarques          #+#    #+#             */
/*   Updated: 2025/11/26 15:44:36 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*get_first_pipe_token(t_token *token, t_token **prev_to_pipe)
{
	t_token	*curr;
	t_token	*prev;

	curr = token;
	prev = NULL;
	*prev_to_pipe = NULL;
	while (curr)
	{
		if (curr->type_tok == T_PIPE)
		{
			*prev_to_pipe = prev;
			if (prev)
				prev->next = NULL;
			return (curr);
		}
		prev = curr;
		curr = curr->next;
	}
	return (NULL);
}

/*
 * set memory allocation for left and right side of the tree
 */
static int	init_pipe_ast_children(t_ast *ast, t_token *pipe,
	t_token *saved_next, t_token *prev)
{
	saved_next = pipe->next;
	pipe->next = NULL;
	ast->type = T_PIPE;
	ast->left = init_ast();
	if (!ast->left)
	{
		if (prev)
			prev->next = pipe;
		pipe->next = saved_next;
		return (-1);
	}
	ast->right = init_ast();
	if (!ast->right)
	{
		if (prev)
			prev->next = pipe;
		pipe->next = saved_next;
		free_ast(ast->left);
		ast->left = NULL;
		return (-1);
	}
	return (0);
}

static int	free_pipe_ast(t_ast *ast)
{
	free_ast(ast->left);
	free_ast(ast->right);
	ast->left = NULL;
	ast->right = NULL;
	return (-1);
}

int	parse_pipe_right(t_token *saved_next,
	t_ast *ast, t_token *pipe)
{
	int	ret;

	pipe->next = NULL;
	ret = create_ast(saved_next, ast->right);
	if (ret < 0)
	{
		free_pipe_ast(ast);
		pipe->next = saved_next;
		return (-1);
	}
	pipe->next = saved_next;
	return (1);
}

/*
 * finds the first | token
 * prev points to the token before the |
 * saved_next points to the token after the |
 * disconnects the | from the list for parsing
 * left/right branches
 * return = 0 -> no pipe found
 * return = 1 -> pipe parsed successfully
 * return = -1 -> error
 */
int	parse_pipe(t_token **token, t_ast *ast)
{
	t_token	*prev;
	t_token	*pipe;
	t_token	*saved_next;

	if (!token || !*token || !ast)
		return (-1);
	pipe = get_first_pipe_token(*token, &prev);
	if (!pipe || !pipe->next)
		return (0);
	saved_next = pipe->next;
	if (init_pipe_ast_children(ast, pipe, saved_next, prev) < 0)
		return (-1);
	if (prev && create_ast(*token, ast->left) < 0)
	{
		free_pipe_ast(ast);
		pipe->next = saved_next;
		return (-1);
	}
	if (!prev)
		ast->left = NULL;
	if (parse_pipe_right(saved_next, ast, pipe) < 0)
		return (-1);
	*token = pipe;
	return (1);
}
