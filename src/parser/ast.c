/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 15:23:32 by jmarques          #+#    #+#             */
/*   Updated: 2025/11/26 15:31:16 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * fill the array with the values of the input
 */
 int	fill_array(t_token *token, t_ast *ast)
{
	t_token	*buf;
	int		i;

	buf = token;
	i = 0;
	while (buf)
	{
		if (buf->type_tok == T_STRING)
		{
			ast->args[i] = ft_strdup(buf->data);
			if (!ast->args[i])
			{
				while (i > 0)
					free(ast->args[--i]);
				return (-1);
			}
			i++;
		}
		buf = buf->next;
	}
	ast->args[i] = NULL;
	return (0);
}

/*
 * create a new array of string to put the value
 
static int	allocate_array(t_token *token, t_ast *ast)
{
	int		count;
	t_token	*buf;

	buf = token;
	count = 0;
	while (buf)
	{
		if (buf->type_tok == T_STRING)
			count++;
		buf = buf->next;
	}
	if (count == 0)
		return (0);
	ast->args = ft_calloc(count + 1, sizeof(char *));
	if (!ast->args)
		return (-1);
	if (fill_array(token, ast) < 0)
	{
		free_string_array(ast->args);
		ast->args = NULL;
		return (-1);
	}
	return (0);
}*/

int	ft_heredoc(t_token *token, t_ast *ast)
{
	if (!token || !ast)
		return (-1);
	if (token->type_tok != T_HEREDOC)
		return (0);
	if (!token->next || !token->next->data)
		return (-1);
	ast->heredocs = malloc(sizeof(t_heredoc));
	if	(!ast->heredocs)	
		return (-1);
	ast->heredocs->delimiter = ft_strdup(token->next->data);
	if (!ast->heredocs->delimiter)
	{
		free(ast->heredocs);
		ast->heredocs = NULL;
		return (-1);
	}
	ast->heredocs->expand = !token->next->quoted;
	ast->heredocs->fd = -1;
	return (1);
}
/*
int	create_ast(t_token *token, t_ast *ast)
{
	if (!token || !ast)
		return (-1);
	int hd_status = ft_heredoc(token, ast);
	if (hd_status == -1) // Erro real de malloc
		return (-1);
	if (parse_pipe(&token, ast))
		return (1);
	if (redirection(&token, ast))
		return (1);
	return (allocate_array(token, ast));
}*/

