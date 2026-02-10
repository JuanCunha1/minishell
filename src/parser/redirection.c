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
/*
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
}*/
/*
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
}*/
/*
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
}*/

void add_redir(t_redir **list, t_redir *new_redir)
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

t_redir *create_redir_from_tokens(t_token **token)
{
    t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
    redir->type = (*token)->type_tok;
    *token = (*token)->next;
    if (!*token)
    {
		free(redir);
		return (NULL);
	}
    redir->file = ft_strdup((*token)->data);
    redir->next = NULL;
    *token = (*token)->next;
    return (redir);
}
t_redir	*create_redir(char *op, char *file)
{
    t_redir	*redir;
	
	redir = malloc(sizeof(t_redir));
    if (!ft_strncmp(op, "<", 2))
        redir->type = T_REDIR_IN;
    else if (!ft_strncmp(op, ">", 2	))
        redir->type = T_REDIR_OUT;
    else if (!ft_strncmp(op, ">>", 2))
        redir->type = T_REDIR_APPEND;
    else if (!ft_strncmp(op, "<<", 2))
        redir->type = T_HEREDOC;
    redir->file = ft_strdup(file);
    if (!redir->file)
    {
        free(redir);
        return (NULL);
    }
    redir->next = NULL;
    return (redir);
}

char	**command(char **args, t_token **token)
{
	int	i;

	i = 0;
	while (*token && (*token)->type_tok == T_STRING)
	{
		args[i++] = ft_strdup((*token)->data);
        if (!args[i - 1])
        {
            while (--i >= 0)
                free(args[i]);
            free(args);
            return (NULL);
        }
        *token = (*token)->next;
	}
	args[i] = NULL;
	return (args);
}

char **parse_command(t_token **token)
{
    char	**args;
    int		size;
	t_token *tmp;

	size = 0;
    tmp = *token;
    while (tmp && tmp->type_tok == T_STRING)
    {
        size++;
        tmp = tmp->next;
    }
    args = malloc(sizeof(char *) * (size + 1));
	if (!args)
		return (NULL);
    return (command(args, token));
}

int	redirection(t_ast *node, t_token **token)
{
	t_redir	*redir;

	while (*token && is_redirection((*token)->type_tok))
    {
        redir = create_redir_from_tokens(token);
        if (!redir)
        {
            free_ast(node);
            return (-1);
        }
        add_redir(&node->redirs, redir);
    }
	return (0);
}

t_ast *parse_redirection(t_token **token)
{
    t_ast	*node;

	node = init_ast();
	if (!node)
		return (NULL);
	if (redirection(node, token) < 0)
		return (NULL);
	node->args = parse_command(token);
    if (redirection(node, token) < 0)
		return (NULL);
    return (node);
}