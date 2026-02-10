/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 12:44:40 by jmarques          #+#    #+#             */
/*   Updated: 2025/11/26 12:44:42 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator(t_type type)
{
	return (type == T_PIPE
		|| type == T_REDIR_OUT
		|| type == T_REDIR_IN
		|| type == T_REDIR_APPEND
		|| type == T_HEREDOC);
}

int	is_redirection(t_type type)
{
	return (type == T_REDIR_OUT
		|| type == T_REDIR_IN
		|| type == T_REDIR_APPEND
		|| type == T_HEREDOC);
}

int	syntax_newline_error(t_shell *sh)
{
	ft_putstr_fd("bash: syntax error near unexpected token 'newline'\n", 2);
	sh->exit_status = 2;
	return (-1);
}

int	syntax_token_error(t_shell *sh, char *tok)
{
	ft_putstr_fd("bash: syntax error near unexpected token '", 2);
	ft_putstr_fd(tok, 2);
	ft_putendl_fd("'", 2);
	sh->exit_status = 2;
	return (-1);
}

int	syntax_error(t_shell *sh, t_token *token)
{
	if (token->type_tok == T_PIPE)
	{
		if (token->next == NULL)
			return (syntax_newline_error(sh));
		if (token->next->type_tok == T_PIPE)
			return (syntax_token_error(sh, token->next->data));
	}
	if (is_redirection(token->type_tok))
	{
		if (token->next == NULL)
			return (syntax_newline_error(sh));
		if (is_operator(token->next->type_tok))
			return (syntax_token_error(sh, token->next->data));
	}
	return (0);
}

int	check_token(t_shell *sh, t_token *token)
{
	t_token *tmp;

	if (token && token->type_tok == T_PIPE)
		return (syntax_token_error(sh, token->data));
	tmp = token;
	while (tmp)
	{
		if (syntax_error(sh, tmp) == -1)
			return (-1);
		tmp = tmp->next;
	}
	return (0);
}
void print_ast(t_ast *node, int level)
{
    int i;

    if (!node)
        return;

    // indentação visual
    i = 0;
    while (i < level)
    {
        printf("  ");
        i++;
    }

    if (node->type == T_PIPE)
        printf("PIPE\n");
    else if (node->type == T_STRING)
        printf("CMD\n");

    // imprimir args
    if (node->args)
    {
        i = 0;
        while (node->args[i])
        {
            int j = 0;
            while (j < level + 1)
            {
                printf("  ");
                j++;
            }
            printf("arg[%d]: %s\n", i, node->args[i]);
            i++;
        }
    }

    // imprimir redirections
    t_redir *r = node->redirs;
    while (r)
    {
        int j = 0;
        while (j < level + 1)
        {
            printf("  ");
            j++;
        }

        if (r->type == T_REDIR_IN)
            printf("redir: < %s\n", r->file);
        else if (r->type == T_REDIR_OUT)
            printf("redir: > %s\n", r->file);
        else if (r->type == T_REDIR_APPEND)
            printf("redir: >> %s\n", r->file);
        else if (r->type == T_HEREDOC)
            printf("redir: << %s\n", r->file);

        r = r->next;
    }

    // recursão
    print_ast(node->left, level + 1);
    print_ast(node->right, level + 1);
}
int	parser(t_shell *sh)
{
	if (check_token(sh, sh->tokens) == -1)
		return (-1);
	sh->ast = init_ast();
	if (!sh->ast)
		return (-1);
	/*if (create_ast(sh->tokens, sh->ast) < 0)
	{
		free_ast(sh->ast);
		sh->ast = NULL;
		return (-1);
	}*/
	sh->ast = parse_pipe(&(sh)->tokens);
	if (!sh->ast)
	{
		free_ast(sh->ast);
		sh->ast = NULL;
		return (-1);
	}

	//print_ast(sh->ast, 0);
	return (1);
}

