/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 10:54:45 by jmarques          #+#    #+#             */
/*   Updated: 2026/02/17 10:54:48 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

void	adding_token(t_token **tokens, t_token *new_token)
{
	t_token	*tmp;

	if (!*tokens)
	{
		*tokens = new_token;
		return ;
	}
	tmp = *tokens;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_token;
}

t_token	*create_token(char *str, t_type type)
{
	t_token	*tok;

	if (!str)
		return (NULL);
	tok = malloc(sizeof(t_token));
	if (!tok)
	{
		free(str);
		return (NULL);
	}
	tok->data = str;
	tok->type_tok = type;
	tok->next = NULL;
	tok->quoted = 0;
	return (tok);
}
