
#include <lexer.h>

static void	emit_operator(t_lexer *lx, int len)
{
    t_token	*tok;

    emit_word(lx);
    tok = create_token(ft_substr(lx->input, lx->i, len),
                       get_operator_type(lx->input + lx->i));
    adding_token(&lx->tokens, tok);
    lx->i += len - 1;
}

static t_lexer	normal_state(t_lexer lx, const char *input, char c)
{
	int	len;

	len = operator_len(input + lx.i);
	if (c == '\'')
		lx.state = LX_IN_SQUOTE;
	else if (c == '"')
		lx.state = LX_IN_DQUOTE;
	else if (c == '\\' && input[lx.i + 1])
	{
		lx.i++;
        buf_add(&lx, input[lx.i]);
	}
	else if (len)
		emit_operator(&lx, len);
	else if (isspace((unsigned char)c))
		emit_word(&lx);
	else if (c == '$')
		buf_add_var(&lx, input);
	else
		buf_add(&lx, c);
	return (lx);
}

static t_lexer	single_quote(t_lexer lx, char c, int quoted)
{
	(void)quoted;
	if (c == '\'')
		lx.state = LX_NORMAL;
	else
		buf_add(&lx, c);
	//lx.tokens->quoted = quoted;
	return (lx);
}

static t_lexer	double_quote(t_lexer lx, char c, const char *input, int quoted)
{
	(void)quoted;
	if (c == '"')
		lx.state = LX_NORMAL;
	else if (c == '\\' && input[lx.i + 1] != '\0'
		&& ft_strchr("\"\\$`", input[lx.i + 1]))
	{
		lx.i++;
		buf_add(&lx, input[lx.i]);
	}
	else if (c == '$')
		buf_add_var(&lx, input);
	else
		buf_add(&lx, c);
	//lx.tokens->quoted = quoted;
	return (lx);
}

t_lexer	states_loop(t_lexer lx, const char *input)
{
	char	c;

	c = input[lx.i];
	if (lx.state == LX_NORMAL)
		lx = normal_state(lx, input, c);
	else if (lx.state == LX_IN_SQUOTE)
		lx = single_quote(lx, c, 1);
	else if (lx.state == LX_IN_DQUOTE)
		lx = double_quote(lx, c, input, 1);
	lx.i++;
	return (lx);
}