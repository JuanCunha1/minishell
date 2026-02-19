/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:00:09 by jmarques          #+#    #+#             */
/*   Updated: 2026/02/19 12:00:00 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_redir(t_redir **list, t_redir *new_redir)
{
	t_redir	*tmp;

	if (!new_redir)
		return ;
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

/*
** Crea una redirección normal (>, >>, <)
*/
static t_redir  *parse_file_redir(t_token **token)
{
	t_redir	*redir;

	if (!*token || !is_redirection((*token)->type_tok)
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

/*
** Parsea un heredoc:
**   << DELIM
** Crea un t_heredoc y lo añade a node->heredocs.
** De momento 'expand' se deja a 1 (expansión activada siempre).
*/
static t_heredoc	*parse_heredoc(t_token **token)
{
	t_heredoc	*hd;
	char		*delim;

	if (!*token || (*token)->type_tok != T_HEREDOC)
		return (NULL);
	*token = (*token)->next;
	if (!*token || (*token)->type_tok != T_STRING)
		return (NULL);
	delim = ft_strdup((*token)->data);
	if (!delim)
		return (NULL);
	hd = create_heredoc(delim, !(*token)->quoted);
	if (!hd)
	{
		free(delim);
		return (NULL);
	}
	*token = (*token)->next;
	return (hd);
}

/*
** Consume una secuencia de redirecciones (incluyendo heredocs)
** y las añade al nodo:
**   - >, >>, <  -> node->redirs
**   - <<	    -> node->heredocs
** En caso de error, libera el nodo y devuelve 0.
*/
static int  redirection(t_token **token, t_ast *node)
{
	t_redir	 	*redir;
	t_heredoc	hd;

	while (*token && is_redirection((*token)->type_tok))
	{
		hd = parse_heredoc(token);
		redir = parse_file_redir(token);
		if ((*token)->type_tok == T_HEREDOC)
		{
			if (!hd)
				break ;
			add_heredoc(&node->heredocs, &hd);
		}
		else
		{
			if (!redir)
				break ;
			add_redir(&node->redirs, redir);
		}
	}
	if (!hd && !redir)
	{
		free_ast(node);
		return (0);
	}
	return (1);
}

/*
** Segmento de comando:
**   [redirs/heredocs] ARGS [redirs/heredocs]
**
** Ejemplos válidos:
**   cmd arg > out
**   < in cmd arg
**   cmd <<EOF | otro
*/
t_ast   *parse_command_segment(t_token **token)
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
		// Comando sin argumentos: permitido si hay heredocs o redirecciones
		node->args = malloc(sizeof(char *));
		if (!node->args)
		{
			free_ast(node);
			return (NULL);
		}
		node->args[0] = NULL;
	}
	if (!redirection(token, node))
		return (NULL);
	node->type = T_STRING;
	return (node);
}
