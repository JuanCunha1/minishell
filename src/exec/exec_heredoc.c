/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 12:03:54 by jmarques          #+#    #+#             */
/*   Updated: 2026/02/10 12:03:55 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <readline/history.h>

char	*expand_string_for_heredoc(const char *input, char **env,
	int last_status)
{
	t_lexer	lx;
	size_t	old_i;

	lx = init_lexer(input, env, last_status);
	if (!lx.buffer)
		return (NULL);
	while (input[lx.i])
	{
		if (input[lx.i] == '$')
		{
			old_i = lx.i;
			buf_add_var(&lx, input);
			if (lx.i == old_i)
				buf_add(&lx, input[lx.i]);
		}
		else
			buf_add(&lx, input[lx.i]);
		lx.i++;
	}
	buf_add(&lx, '\0');
	free_tokens(lx.tokens);
	free_string_array(lx.env);
	return (lx.buffer);
}

int	loop_heredoc(int expand, char *delimiter, char **envp, int fd)
{
	char	*tmp;
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", 1);
			ft_putstr_fd(delimiter, 1);
			ft_putendl_fd("')", 1);
			return(0);
		}
		if (!ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1))
		{
			free(line);
			return (0);
		}
		if (expand)
		{
			tmp = expand_string_for_heredoc(line, envp, g_signal);
			free(line);
			if (!tmp)
				return (-1);
			line = tmp;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
}

int	read_heredoc(char *delimiter, int expand, char **envp)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipefd) == -1)
		return (-1);
	pid = fork();
	if (pid == 0)
	{
		close(pipefd[0]);
		set_signals_heredoc();
		if(loop_heredoc(expand, delimiter, envp, pipefd[1]) == -1)
			exit(1);
		close(pipefd[1]);
		exit(0);
	}
	close(pipefd[1]);
	if (WIFSIGNALED(status))
    {
        close(pipefd[0]);
        return (-1);
    }
    return (pipefd[0]);
}

int	prepare_heredocs(t_ast *ast, char **envp)
{
	t_redir	*r;

	if (!ast)
		return (0);
	if (ast->type == T_PIPE)
	{
		if (prepare_heredocs(ast->left, envp))
			return (1);
		if (prepare_heredocs(ast->right, envp))
			return (1);
		return (0);
	}
	r = ast->redirs;
	while (r)
	{
		if (r->type == T_HEREDOC)
		{
			r->fd = read_heredoc(r->file, r->expand, envp);
			if (r->fd < 0)
				return (1);
		}
		r = r->next;
	}
	return (0);
}
