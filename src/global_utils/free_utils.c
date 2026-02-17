/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 12:29:07 by jmarques          #+#    #+#             */
/*   Updated: 2026/02/17 12:29:08 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redirs(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir->next;
		free(redir->file);
		free(redir);
		redir = tmp;
	}
}

void	free_pipe_list(t_pipe *head)
{
	t_pipe	*tmp;

	while (head)
	{
		if (head->input_fd > 2)
			close(head->input_fd);
		if (head->pipe_fd[0] > 2)
			close(head->pipe_fd[0]);
		if (head->pipe_fd[1] > 2)
			close(head->pipe_fd[1]);
		tmp = head->next;
		free(head);
		head = tmp;
	}
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
	node->redirs = NULL;
	free(node);
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
