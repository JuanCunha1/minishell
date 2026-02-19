/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 10:05:00 by jmarques          #+#    #+#             */
/*   Updated: 2026/02/19 10:05:00 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Crea un nodo heredoc con:
** - delimiter: delimitador tal cual
** - expand: 1 si debe expandir variables, 0 si no
** - fd: -1 hasta que exec_heredoc lo rellene
*/
t_heredoc   *create_heredoc(char *delimiter, int expand)
{
    t_heredoc   hd;

    hd = malloc(sizeof(t_heredoc));
    if (!hd)
        return (NULL);
    hd.delimiter = ft_strdup(delimiter);
    hd.expand = expand; //0 no expandir / 1 expandir envp
    hd.fd = -1;
    hd.next = NULL;
    return (hd);
}

/*
** Añade un heredoc al final de la lista
*/
void    add_heredoc(t_heredoc **list, t_heredoc *new_hd)
{
    t_heredoc   *tmp;

    if (!list || !new_hd)
        return ;
    if (!*list)
    {
        *list = new_hd;
        return ;
    }
    tmp = *list;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new_hd;
}
