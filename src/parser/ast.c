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

static char	**parse_args_util(t_token **token, char **args)
{
	int		i;
	t_token	*start;

	start = *token;
	i = 0;
	while (*token && (*token)->type_tok == T_CMD)
	{
		args[i] = ft_strdup((*token)->data);
		if (!args[i])
		{
			while (i > 0)
				free(args[--i]);
			free(args);
			*token = start;
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
	char	**args;

	size = 0;
	tmp = *token;
	while (tmp && tmp->type_tok == T_CMD)
	{
		size++;
		tmp = tmp->next;
	}
	args = malloc(sizeof(char *) * (size + 1));
	if (!args)
		return (NULL);
	return (parse_args_util(token, args));
}
