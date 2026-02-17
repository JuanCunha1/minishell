/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_lexer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rubmedin <rubmedin@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 15:14:46 by rubmedin          #+#    #+#             */
/*   Updated: 2025/10/07 18:39:17 by rubmedin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_lexer init_lexer(const char *input, char **env, int exit_status)
{
	t_lexer lx;

	lx.input = input;
	lx.i = 0;
	lx.state = LX_NORMAL;
	lx.tokens = NULL;
	lx.env = envp_dup(env);
	lx.buf_cap = 32;
	lx.buffer = malloc(lx.buf_cap);
	if (!lx.buffer)
        lx.state = LX_ERROR;
	lx.buf_len = 0;
	lx.last_exit_status = exit_status;
	return (lx);
}

int	lexer_error(t_lexer *lx)
{
	if (lx->state != LX_IN_SQUOTE && lx->state != LX_IN_DQUOTE)
		return (1);
	if (lx->state == LX_IN_SQUOTE)
		ft_putendl_fd("bash: unclosed quotes '", 2);
	else if (lx->state == LX_IN_DQUOTE)
		ft_putendl_fd("bash: unclosed quotes \"", 2);
	lx->last_exit_status = 2;
	free(lx->buffer);
	free_tokens(lx->tokens);
	free_string_array(lx->env);
	return (0);
}

t_token *lexer(const char *input, char **env, int exit_status)
{
	t_lexer lx;

	lx = init_lexer(input, env, exit_status);
	if (lx.state  == LX_ERROR)
		return (NULL);
	while (input[lx.i])
		lx = states_loop(lx, input);
	if (lexer_error(&lx) == 0)
		return (NULL);
	emit_word(&lx);
	free(lx.buffer);
	free_string_array(lx.env);
	return (lx.tokens);
}