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

t_ast	*pipe_pa(t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = init_ast();
	if (!node)
	{
		free_ast(left);
		free_ast(right);
		return (NULL);
	}
	node->type = T_PIPE;
	node->left = left;
	node->right = right;
	return (node);
}

t_ast	*parse_pipe(t_token **token)
{
	t_ast	*left;
	t_ast	*right;

	left = parse_command_segment(token);
	if (!left)
		return (NULL);
	while (*token && (*token)->type_tok == T_PIPE)
	{
		*token = (*token)->next;
		right = parse_command_segment(token);
		if (!right)
		{
			free_ast(left);
			return (NULL);
		}
		left = pipe_pa(left, right);
		if (!left)
			return (NULL);
	}
	return (left);
}
