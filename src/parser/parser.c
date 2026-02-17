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

t_ast	*parser(t_shell *sh)
{
	t_token	*start;

	start = sh->tokens;
	sh->ast = parse_pipe(&sh->tokens);
	free_tokens(start);
	return (sh->ast);
}
