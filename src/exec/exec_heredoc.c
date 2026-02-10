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

#include "minishell.h"
#include <readline/history.h>

int	expand_variables_in_line(char **line, t_shell *sh)
{
	char	*expanded;

	expanded = get_env_value(*line, sh->envp);
	free(*line);
	if (!expanded)
		return (-1);
	*line = expanded;
	return (0);
}

char	*expand_content(char *line, t_shell *sh)
{
	char	*new_line;
	int		i;
	char	*expanded;

	new_line = ft_strdup("");
	i = -1;
	while (line[++i])
	{
		if (line[i] == '$' && line[i + 1])
		{
			expanded = get_env_value(line + i + 1, sh->envp);
			if (expanded)
			{
				new_line = ft_strjoin(new_line, expanded);
				free(expanded);
				while (line[i] && line[i] != '$')
					i++;
			}
		}
	}
	return (new_line);
}

int	loop_heredoc(int fd, t_heredoc *hd, t_shell *sh)
{
	char	*line;
	char	*expanded;

	line = readline("> ");
	if (!line)
	{
		ft_putendl_fd("minishell: warning: heredoc delimited by EOF", 2);
		return (-1);
	}
	if (ft_strncmp(line, hd->delimiter, ft_strlen(hd->delimiter) + 1) == 0)
	{
		free(line);
		return (0);
	}
	expanded = line;
	if (hd->expand)
	{
		printf("Expanding line: %s\n", line);
		expanded = expand_content(line, sh);
		free(line);
		if (!expanded)
			return (-1);
	}
	ft_putendl_fd(expanded, fd);
	free(expanded);
	return (1);
}

int	read_heredoc(t_heredoc *hd, t_shell *sh)
{
	int	pipefd[2];
	int	ret;

	printf("Entrou no read\n");
	if (!hd || !hd->delimiter)
		return (-1);
	if (pipe(pipefd) == -1)
		return (-1);
	while (1)
	{
		ret = loop_heredoc(pipefd[1], hd, sh);
		if (ret <= 0)
			break ;
	}
	close(pipefd[1]);
	hd->fd = pipefd[0];
	if (ret < 0)
		return (-1);
	return (0);
}

int	process_heredocs(t_ast *ast, t_shell *sh)
{
	if (!ast)
		return (0);
	if (ast->heredocs)
	{
		printf("Entrou no read\n");
		if (read_heredoc(ast->heredocs, sh) != 0)
			return (-1);
	}
	if (process_heredocs(ast->left, sh) != 0)
		return (-1);
	if (process_heredocs(ast->right, sh) != 0)
		return (-1);
	return (0);
}
