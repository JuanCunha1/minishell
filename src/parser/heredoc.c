#include "minishell.h"

int	ft_heredoc(t_shell *sh)
{
	if (!sh || !sh->tokens)
		return (-1);
	if (sh->tokens->type_tok != T_HEREDOC)
		return (0);
	if (!sh->tokens->next || !sh->tokens->next->data)
		return (-1);
	sh->heredocs = malloc(sizeof(t_heredoc));
	if	(!sh->heredocs)
		return (-1);
	sh->heredocs->delimiter = ft_strdup(sh->tokens->next->data);
	if (!sh->heredocs->delimiter)
	{
		free(sh->heredocs);
		return (-1);
	}
	sh->heredocs->fd = -1;
	printf("Delimiter: %s, quoted: %d\n", sh->heredocs->delimiter, sh->tokens->next->quoted);
	sh->heredocs->expand = sh->tokens->next->quoted;
	return (1);
}