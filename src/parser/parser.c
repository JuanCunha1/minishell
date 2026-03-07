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
