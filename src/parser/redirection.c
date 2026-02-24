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

void	add_redir(t_redir **list, t_redir *new_redir)
{
	t_redir	*tmp;

	new_redir->next = NULL;
	if (!*list)
	{
		*list = new_redir;
		return ;
	}
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_redir;
}

static int	fill_redir_arg(t_redir *redir, t_token *arg)
{
	if (!arg || arg->type_tok != T_CMD)
		return (0);
	if (redir->type == T_HEREDOC)
		redir->expand = !arg->quoted;
	redir->file = ft_strdup(arg->data);
	if (!redir->file)
		return (0);
	return (1);
}

static t_redir	*parse_redir(t_token **token)
{
	t_redir	*redir;
	t_token	*op;
	t_token	*arg;

	if (!*token || !is_redirection((*token)->type_tok))
		return (NULL);
	op = *token;
	arg = op->next;
	redir = init_redir(op->type_tok);
	if (!redir)
		return (NULL);
	if (!fill_redir_arg(redir, arg))
	{
		free(redir);
		return (NULL);
	}
	*token = arg->next;
	return (redir);
}

static int	redirection(t_token **token, t_ast *node)
{
	t_redir	*redir;

	while (*token && is_redirection((*token)->type_tok))
	{
		redir = parse_redir(token);
		if (!redir)
		{
			free_ast(node);
			return (0);
		}
		add_redir(&node->redirs, redir);
	}
	return (1);
}

t_ast	*parse_command_segment(t_token **token)
{
	t_ast	*node;

	node = init_ast();
	if (!node)
		return (NULL);
	if (!redirection(token, node))
		return (NULL);
	node->args = parse_args(token);
	if (!node->args)
	{
		free_ast(node);
		return (NULL);
	}
	if (!redirection(token, node))
		return (NULL);
	node->type = T_CMD;
	return (node);
}
