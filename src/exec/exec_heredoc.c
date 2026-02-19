/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 10:00:00 by jmarques          #+#    #+#             */
/*   Updated: 2026/02/19 10:00:00 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>

/*
** Lee líneas desde stdin hasta encontrar el delimitador.
** Escribe el contenido en el extremo de escritura del pipe (write_fd).
** De momento no hacemos expansión POSIX ni manejo avanzado de señales.
** Devuelve 0 en éxito, -1 en error.
*/
static int  read_heredoc_into_fd(int write_fd, t_heredoc *hd)
{
	char    *line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{

			//modificar esto para la mesma mensaje de lo bash
			// usando el ctrl + d
			// algo asi: bash: warning: here-document at line 18 delimited by end-of-file (wanted `a')
			ft_putendl_fd("minishell: warning: heredoc delimited by EOF", 2);
			return (-1);
		}
		if (ft_strncmp(line, hd->delimiter,
				ft_strlen(hd->delimiter) + 1) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, write_fd);
		free(line);
	}
	return (0);
}

/*
** Implementación pública: lee un heredoc y deja el fd de lectura en hd->fd.
** Crea un pipe, escribe el contenido en el write-end y guarda el read-end.
*/
int read_heredoc(t_heredoc *hd, char ***envp)
{
	int     pipefd[2];
	int     ret;


	if (!hd || !hd->delimiter)
		return (-1);
	if (pipe(pipefd) == -1)
		return (-1);
	ret = read_heredoc_into_fd(pipefd[1], hd);
	close(pipefd[1]);
	if (ret < 0)
	{
		close(pipefd[0]);
		return (-1);
	}
	hd->fd = pipefd[0];
	return (0);
}

/*
** Aplica el heredoc de un nodo concreto:
** - Si hay varios heredocs, por ahora usamos el ÚLTIMO (comportamiento típico).
** - Redirigimos STDIN al fd del heredoc.
*/
static int  apply_node_heredoc(t_ast *ast, char ***envp)
{
	t_heredoc   *hd;
	t_heredoc   *last;

	if (!ast || !ast->heredocs)
		return (0);
	hd = ast->heredocs;
	last = hd;
	while (hd)
	{
		last = hd;
		hd = hd->next;
	}
	if (last->fd < 0)
		return (-1);
	if (redirect_fd(last->fd, STDIN_FILENO) < 0)
	{
		close(last->fd);
		return (-1);
	}
	return (0);
}

/*
** Recorre el AST y:
** 1) Para cada nodo con heredocs, llama a read_heredoc() para prepararlos.
** 2) Aplica la redirección de stdin al heredoc correspondiente.
**
** De momento:
** - No gestionamos señales especiales.
** - No hacemos expansión de variables.
*/
int process_heredocs(t_ast *ast, char ***envp)
{
	t_heredoc   *hd;

	if (!ast)
		return (0);
	if (process_heredocs(ast->left, envp) != 0)
		return (-1);
	if (process_heredocs(ast->right, envp) != 0)
		return (-1);
	hd = ast->heredocs;
	while (hd)
	{
		if (read_heredoc(hd, envp) != 0)
			return (-1);
		hd = hd->next;
	}
	if (ast->heredocs)
	{
		if (apply_node_heredoc(ast, envp) != 0)
			return (-1);
	}
	return (0);
}
