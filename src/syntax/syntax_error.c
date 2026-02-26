/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 12:07:45 by jmarques          #+#    #+#             */
/*   Updated: 2026/02/17 12:07:47 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	syntax_newline_error(void)
{
	ft_putendl_fd("minishell: syntax error near unexpected token 'newline'", 2);
	return (2);
}

static int	syntax_token_error(char *tok)
{
	ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
	ft_putstr_fd(tok, 2);
	ft_putendl_fd("'", 2);
	return (2);
}

static int	syntax_error(t_token *token)
{
	if (token->type_tok == T_PIPE)
	{
		if (token->next == NULL)
			return (syntax_newline_error());
		if (token->next->type_tok == T_PIPE)
			return (syntax_token_error(token->next->data));
	}
	if (is_redirection(token->type_tok))
	{
		if (token->next == NULL)
			return (syntax_newline_error());
		if (is_operator(token->next->type_tok))
			return (syntax_token_error(token->next->data));
	}
	return (0);
}

int	check_tokens(t_token *token)
{
	t_token	*tmp;

	if (token && token->type_tok == T_PIPE)
		return (syntax_token_error(token->data));
	tmp = token;
	while (tmp)
	{
		if (syntax_error(tmp) != 0)
			return (2);
		tmp = tmp->next;
	}
	return (0);
}
