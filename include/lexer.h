#ifndef LEXER_H
# define LEXER_H

#include <minishell.h>

enum e_token_type
{
	
	CHAR_PIPE = '|',
	CHAR_AMPERSAND = '&',
	CHAR_QUOTE = '\'',
	CHAR_DQUOTE = '\"',
	CHAR_SEMICOLON = ';',
	CHAR_WHITESPACE = ' ',
	CHAR_ESCAPESEQUENCE = '\\',
	CHAR_TAB = '\t',
	CHAR_NEWLINE = '\n',
	CHAR_GREATER = '>',
	CHAR_LESSER = '<',
	CHAR_DOLLAR = '$',
	CHAR_NULL = 0,
	CHAR_GENERAL = -1,
	TOKEN = -2,
};

typedef struct s_token
{
	enum e_token_type	type;
	char 				*value;
	struct s_token		*next;
} t_token;

t_token	*create_token(enum e_token_type type, const char *value);
enum e_token_type		getchartype(char c);
int lexer_doble(char c, char c1, t_token *token);

int lexer_util(char c, t_token *token);

int	lexer_quotes(char *str);

t_token	*lexer(char *str);

void	lst_add_token_back(t_token **t, t_token *new_token);

void	lst_token_clear(t_token **t, void (*del)(void *));
#endif