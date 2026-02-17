/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 10:51:59 by jmarques          #+#    #+#             */
/*   Updated: 2026/02/17 10:52:01 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

t_type	get_operator_type(const char *s)
{
	if (s[0] == '>' && s[1] == '>')
		return (T_REDIR_APPEND);
	if (s[0] == '<' && s[1] == '<')
		return (T_HEREDOC);
	if (s[0] == '>')
		return (T_REDIR_OUT);
	if (s[0] == '<')
		return (T_REDIR_IN);
	if (s[0] == '|')
		return (T_PIPE);
	return (T_STRING);
}

void	emit_word(t_lexer *lx)
{
	t_token	*tok;

	if (lx->buf_len == 0)
		return ;
	buf_add(lx, '\0');
	tok = create_token(ft_strdup(lx->buffer), T_STRING);
	adding_token(&lx->tokens, tok);
	lx->buf_len = 0;
}

void	buf_add(t_lexer *lx, char c)
{
	char	*tmp;

	if (lx->buf_len + 1 >= lx->buf_cap)
	{
		lx->buf_cap *= 2;
		tmp = realloc(lx->buffer, lx->buf_cap);
		if (!lx->buffer)
		{
			free(lx->buffer);
			return ;
		}
		lx->buffer = tmp;
	}
	lx->buffer[lx->buf_len++] = c;
}

int	operator_len(const char *s)
{
	if (s[0] == '>' && s[1] == '>')
		return (2);
	if (s[0] == '<' && s[1] == '<')
		return (2);
	if (s[0] == '>' || s[0] == '<' || s[0] == '|')
		return (1);
	return (0);
}
