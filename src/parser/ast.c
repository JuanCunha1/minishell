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

static char	**parse_args_util(t_token **token, int size)
{
	char	**args;
	int		i;

	args = malloc(sizeof(char *) * (size + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (*token && (*token)->type_tok == T_STRING)
	{
		args[i] = ft_strdup((*token)->data);
		if (!args[i])
		{
			while (i-- > 0)
				free(args[i]);
			free(args);
			return (NULL);
		}
		i++;
		*token = (*token)->next;
	}
	args[i] = NULL;
	return (args);
}

char	**parse_args(t_token **token)
{
	int		size;
	t_token	*tmp;

	size = 0;
	tmp = *token;
	while (tmp && tmp->type_tok == T_STRING)
	{
		size++;
		tmp = tmp->next;
	}
	return (parse_args_util(token, size));
}
