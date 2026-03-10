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

static int	expand_args(t_ast *node)
{
	char	**new_args;
	int		i;

	new_args = malloc(sizeof(char *) * node->args_cap * 2);
	if (!new_args)
		return (0);
	i = 0;
	while (i < node->args_count)
	{
		new_args[i] = node->args[i];
		i++;
	}
	free(node->args);
	node->args = new_args;
	node->args_cap *= 2;
	return (1);
}

int	add_arg(t_ast *node, char *arg)
{
	if (node->args_count + 1 >= node->args_cap)
		if (!expand_args(node))
			return (0);
	node->args[node->args_count] = ft_strdup(arg);
	if (!node->args[node->args_count])
		return (0);
	node->args_count++;
	node->args[node->args_count] = NULL;
	return (1);
}

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

t_ast	*parser(t_shell *sh)
{
	t_token	*start;

	start = sh->tokens;
	sh->ast = parse_pipe(&sh->tokens);
	free_tokens(start);
	return (sh->ast);
}
