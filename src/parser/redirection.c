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

static t_redir	*parse_redir(t_token **token)
{
	t_redir	*redir;

	if (!*token || !is_redirection((*token)->type_tok))
		return (NULL);
	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = (*token)->type_tok;
	redir->next = NULL;
	*token = (*token)->next;
	if (!*token || (*token)->type_tok != T_STRING)
	{
		free(redir);
		return (NULL);
	}
	redir->file = ft_strdup((*token)->data);
	if (!redir->file)
	{
		free(redir);
		return (NULL);
	}
	*token = (*token)->next;
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
	if (!redirection(token, node))
		return (NULL);
	node->type = T_STRING;
	return (node);
}
