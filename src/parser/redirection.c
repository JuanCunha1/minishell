/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:00:09 by jmarques          #+#    #+#             */
/*   Updated: 2025/11/26 16:00:10 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*get_last_redirection_token(t_token *token,
	t_token **prev_to_redir)
{
	t_token	*curr;
	t_token	*prev;
	t_token	*last;
	t_token	*last_prev;

	curr = token;
	prev = NULL;
	last_prev = NULL;
	last = NULL;
	*prev_to_redir = NULL;
	while (curr)
	{
		if (curr->type_tok >= T_REDIR_OUT && curr->type_tok <= T_HEREDOC)
		{
			last = curr;
			last_prev = prev;
		}
		prev = curr;
		curr = curr->next;
	}
	*prev_to_redir = last_prev;
	return (last);
}

static int	parse_redirection_target(t_token **token, t_ast *ast,
								t_token **prev, t_token **saved_next)
{
	t_token	*redir;

	*prev = NULL;
	*saved_next = NULL;
	redir = get_last_redirection_token(*token, prev);
	if (!redir)
		return (0);
	if (!redir->next || redir->next->type_tok != T_STRING)
		return (-1);
	ast->type = redir->type_tok;
	ast->file = ft_strdup(redir->next->data);
	if (!ast->file)
		return (-1);
	if (*prev)
	{
		*saved_next = (*prev)->next;
		(*prev)->next = redir->next->next;
	}
	else
	{
		*saved_next = *token;
		*token = redir->next->next;
	}
	return (1);
}

int	rollback_redirection(t_token **token, t_ast *ast,
					t_token **prev, t_token **saved_next)
{
	if (!ast)
		return (-1);
	if (ast->left)
	{
		free_ast(ast->left);
		ast->left = NULL;
	}
	free(ast->file);
	ast->file = NULL;
	if (*prev)
		(*prev)->next = *saved_next;
	else
		*token = *saved_next;
	return (-1);
}

int	redirection(t_token **token, t_ast *ast)
{
	t_token	*prev;
	t_token	*saved_next;
	int		ret;

	ret = parse_redirection_target(token, ast, &prev, &saved_next);
	if (ret <= 0)
		return (ret);
	ast->left = init_ast();
	if (!ast->left)
		return (rollback_redirection(token, ast, &prev, &saved_next));
	ret = create_ast(*token, ast->left);
	if (ret < 0)
		return (rollback_redirection(token, ast, &prev, &saved_next));
	if (prev)
		prev->next = saved_next;
	else
		*token = saved_next;
	return (1);
}
