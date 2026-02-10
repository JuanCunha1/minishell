#include "minishell.h"

void free_heredocs(t_heredoc *heredoc)
{
    t_heredoc *tmp;

    while (heredoc)
    {
        tmp = heredoc->next;
        free(heredoc->delimiter);
        free(heredoc);
        heredoc = tmp;
    }
}

void free_redirs(t_redir *redir)
{
    t_redir *tmp;

    while (redir)
    {
        tmp = redir->next;
        free(redir->file);
        free(redir);
        redir = tmp;
    }
}
void	*free_pipe_list(t_pipe *head)
{
	t_pipe *tmp;

	while (head)
	{
		tmp = head->next;
		if (head->input_fd != -1 && head->input_fd != STDIN_FILENO)
			close(head->input_fd);
		if (head->pipe_fd[0] != -1)
			close(head->pipe_fd[0]);
		if (head->pipe_fd[1] != -1)
			close(head->pipe_fd[1]);
		free(head);
		head = tmp;
	}
	return (NULL);
}
void	free_ast(t_ast *node)
{
	int	i;

	i = 0;
	if (!node)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	if (node->args)
	{
		while (node->args[i])
			free(node->args[i++]);
		free(node->args);
		node->args = NULL;
	}

	free_redirs(node->redirs);
	//free_heredocs(node->heredocs);
	node->redirs = NULL;
	//node->heredocs = NULL;
	free(node);
}

void	free_shell(t_shell *sh)
{
	if (!sh)
		return ;
	if (sh->str)
	{
		free(sh->str);
		sh->str = NULL;
	}
	if (sh->tokens)
	{
		free_tokens(sh->tokens);
		sh->tokens = NULL;
	}
	if (sh->ast)
	{

		free_ast(sh->ast);
		sh->ast = NULL;
	}
}

void	free_tokens(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token->next;
		free(token->data);
		free(token);
		token = tmp;
	}
	token = NULL;
}

void	free_string_array(char **str)
{
	int	i;

	i = 0;
	if (!str)
		return ;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
	str = NULL;
}
