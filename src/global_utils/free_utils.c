/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Libera la lista de redirecciones normales: >, >>, <
*/
void    free_redirs(t_redir *redir)
{
    t_redir *tmp;

    while (redir)
    {
        tmp = redir->next;
        free(redir->file);
        free(redir);
        redir = tmp;
    }
}

/*
** Libera la lista de heredocs:
** - delimiter (ft_strdup)
** - nodo t_heredoc
** - NO cierra fd aquí (lo hace exec_heredoc)
*/
void    free_heredocs(t_heredoc *hd)
{
    t_heredoc   *tmp;

    while (hd)
    {
        tmp = hd->next;
        if (hd->delimiter)
            free(hd->delimiter);
        free(hd);
        hd = tmp;
    }
}

/*
** Libera la lista de pipes creada en build_pipe_list()
*/
void    free_pipe_list(t_pipe *head)
{
    t_pipe  *tmp;

    while (head)
    {
        if (head->input_fd > 2)
            close(head->input_fd);
        if (head->pipe_fd[0] > 2)
            close(head->pipe_fd[0]);
        if (head->pipe_fd[1] > 2)
            close(head->pipe_fd[1]);
        tmp = head->next;
        free(head);
        head = tmp;
    }
}

/*
** Libera un nodo AST completo:
** - Recursión en left/right
** - args
** - redirs
** - heredocs (AÑADIDO)
*/
void    free_ast(t_ast *node)
{
    int i;

    if (!node)
        return ;

    free_ast(node->left);
    free_ast(node->right);

    if (node->args)
    {
        i = 0;
        while (node->args[i])
            free(node->args[i++]);
        free(node->args);
        node->args = NULL;
    }

    free_redirs(node->redirs);
    node->redirs = NULL;

    free_heredocs(node->heredocs);
    node->heredocs = NULL;

    free(node);
}

/*
** Libera la lista de tokens generada por el lexer
*/
void    free_tokens(t_token *token)
{
    t_token *tmp;

    while (token)
    {
        tmp = token->next;
        free(token->data);
        free(token);
        token = tmp;
    }
}

/*
** Libera un array de strings terminado en NULL
*/
void    free_string_array(char **str)
{
    int i;

    if (!str)
        return ;
    i = 0;
    while (str[i])
    {
        free(str[i]);
        i++;
    }
    free(str);
}
