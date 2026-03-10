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

int	add_arg(t_ast *node, char *arg)
{
	int		i;
	char	**tmp;

	i = 0;
	if (node->args)
		while (node->args[i])
			i++;
	tmp = realloc(node->args, sizeof(char *) * (i + 2));
	if (!tmp)
		return (0);
	node->args = tmp;
	node->args[i] = ft_strdup(arg);
	if (!node->args[i])
		return (0);
	node->args[i + 1] = NULL;
	return (1);
}
