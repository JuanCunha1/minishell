/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 10:32:03 by jmarques          #+#    #+#             */
/*   Updated: 2026/02/17 10:35:40 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <minishell.h>

typedef struct s_token	t_token;
typedef enum e_type		t_type;

typedef enum e_lex_state
{
	LX_NORMAL,
	LX_IN_SQUOTE,
	LX_IN_DQUOTE,
	LX_ERROR
}	t_lex_state;

typedef struct s_lexer
{
	const char	*input;
	size_t		i;
	t_lex_state	state;
	char		*buffer;
	size_t		buf_len;
	t_token		*tokens;
	char		**env;
	size_t		buf_cap;
	int			had_quotes;
	int			last_exit_status;
}	t_lexer;

t_lexer	init_lexer(const char *input, char **env, int exit_status);

void	states_loop(t_lexer *lx, const char *input);

t_token	*create_token(char *str, t_type type);
void	adding_token(t_token **tokens, t_token *new_token);

void	emit_word(t_lexer *lx);
void	buf_add(t_lexer *lx, char c);
int		operator_len(const char *s);
t_type	get_operator_type(const char *s);

void	buf_add_var(t_lexer *lx, const char *input);
#endif
