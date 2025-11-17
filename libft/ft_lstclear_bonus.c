/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:54:23 by jmarques          #+#    #+#             */
/*   Updated: 2025/05/05 16:54:24 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*tmp;
	t_list	*aux;

	if (!lst || !*lst)
		return ;
	aux = *lst;
	while (aux)
	{
		tmp = aux->next;
		if (del)
			del(aux->content);
		free(aux);
		aux = tmp;
	}
	*lst = NULL;
}
