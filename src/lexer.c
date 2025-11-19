
#include "lexer.h"

enum e_token_type	getchartype(char c)
{
	if (c == '\'')
		return (CHAR_QUOTE);
	if (c == '\"')
		return (CHAR_DQUOTE);
	if (c == '|')
		return (CHAR_PIPE);
	if (c == '&')
		return (CHAR_AMPERSAND);
	if (c == ';')
		return (CHAR_SEMICOLON);
	if (c == '\\')
		return (CHAR_ESCAPESEQUENCE);
	if (c == ' ')
		return (CHAR_WHITESPACE);
	if (c == '\t')
		return (CHAR_TAB);
	if (c == '\n')
		return (CHAR_NEWLINE);
	if (c == '>')
		return (CHAR_GREATER);
	if (c == '<')
		return (CHAR_LESSER);
	if (c == '$')
		return (CHAR_DOLLAR);
	if (c == 0)
		return (CHAR_NULL);
	return (CHAR_GENERAL);
}

int lexer_doble(char c, char c1, t_token *token)
{
	char	op[3];

	if ((c == '>' && c1 == '>') ||
		(c == '<' && c1 == '<'))
	{
		op[0] = c;
		op[1] = c1;
		op[2] = '\0';
		lst_add_token_back(&token, create_token(getchartype(c), op));
		return (2);
	}
	return (0);
}
int lexer_util(char c, t_token *token)
{
	char	op[2];

	if (getchartype(c) == CHAR_WHITESPACE)
		return (1);
	if (getchartype(c) == CHAR_PIPE ||
		getchartype(c) == CHAR_GREATER ||
		getchartype(c) == CHAR_LESSER)
	{
		op[0] = c;
		op[1] = '\0';
		lst_add_token_back(&token, create_token(getchartype(c), op));
		return (1);
	}
	return (0);
}

//int	lexer_dquotes(char *str, t_token *token, int i)
// {
// 	return 0;
// }
int	lexer_quotes(char *str, t_token *token)
{
	int					i;
	int					start;
	enum e_token_type	type;
	
	i = 0;
	if (!str || (str[i] != CHAR_QUOTE && str[i] != CHAR_DQUOTE))
		return (-1);
	type = str[i++];
	start = i; 
	while (str[i] && str[i] != type)
	{
		if (type == CHAR_DQUOTE && str[i] == '$')
		{
			if (i > start)
				lst_add_token_back(&token, create_token(CHAR_GENERAL, ft_substr(str, start, i - start)));
			lst_add_token_back(&token, create_token(CHAR_DOLLAR, "$"));
			i++;
			start = i;
			continue ;
		}
		i++;
	}
	if (i > start)
		lst_add_token_back(&token, create_token(CHAR_GENERAL, ft_substr(str, start, i - start)));
	if (str[i] != type)
		return (-2);
	 return (i + 1);
}

t_token	*lexer(char *str)
{
	int		i;
	t_token	*token;
	int		start;
	
	token = NULL;
	i = 0;
	while (str[i] != '\0')
	{
		start = i;
		i += lexer_util(str[i], token) + lexer_doble(str[i],str[i + 1], token);
		if (start != i)
			continue ;
		if (getchartype(str[i]) != CHAR_GENERAL)
		{
			lst_add_token_back(&token,
				create_token(str[i], ft_substr(str, i, 1)));
			i++;
			continue;
		}
		while (str[i] != '\0' && getchartype(str[i]) == CHAR_GENERAL)
			i++;
		lst_add_token_back(&token,
			create_token(CHAR_GENERAL, ft_substr(str, start, i - start)));
	}
	return (token);
}

t_token	*create_token(enum e_token_type type, const char *value)
{
	t_token	*t;
	
	t = malloc(sizeof(t_token));
	if (!t)
		return (NULL);
	t->type = type;
	t->next = NULL;
	if (value != NULL)
		t->value = ft_strdup(value);
	else
		t->value = ft_strdup("");
	return (t);
}

void	lst_add_token_back(t_token **t, t_token *new_token)
{
	t_token	*temp;

	if (!t || !new_token)
		return;
	if (*t == NULL)
	{
		*t = new_token;
		return;
	}
	temp = *t;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new_token;
}

void	lst_token_clear(t_token **t, void (*del)(void *))
{
	t_token *tmp;
	t_token *aux;

	if (!t || !*t)
		return;
	aux = *t;
	while (aux)
	{
		tmp = aux->next;
		if (del)
			del(aux->value);
		free(aux);
		aux = tmp;
	}
	*t = NULL;
}
