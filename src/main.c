/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rubmedin <rubmedin@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:25:43 by rubmedin          #+#    #+#             */
/*   Updated: 2025/10/07 19:36:20 by rubmedin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/history.h>

volatile sig_atomic_t	g_signal = 0;

static void	multiple_line(t_shell *sh, char **lines)
{
	int	i;

	i = 0;
	while (lines[i])
	{
		sh->tokens = lexer(lines[i], sh->envp, sh->exit_status);
		if (!sh->tokens)
		{
			i++;
			continue ;
		}
		if (check_tokens(sh->tokens) == 2)
		{
			sh->exit_status = 2;
			i++;
			continue ;
		}
		sh->ast = parser(sh);
		sh->exit_status = execute_ast(sh->ast, &sh->envp, sh->exit_status);
		free_shell(sh);
		i++;
	}
}

static int	shell_loop(t_shell *sh)
{
	char	**lines;

	if (g_signal == 130)
		sh->exit_status = 130;
	if (sh->str[0] == '\0')
		return (1);
	lines = ft_split(sh->str, '\n');
	free(sh->str);
	sh->str = NULL;
	if (!lines)
		return (1);
	multiple_line(sh, lines);
	free_string_array(lines);
	return (1);
}

static int	ft_non_interactive(t_shell *shell)
{
	char	*tmp;

	shell->str = get_next_line(0);
	if (!shell->str)
	{
		return (0);
	}
	tmp = ft_strtrim(shell->str, "\n");
	free(shell->str);
	shell->str = tmp;
	return (1);
}

static int	ft_interactive(t_shell *shell)
{
	char	*prompt;

	set_signals_prompt();
	prompt = write_pwd(shell->envp);
	if (!prompt)
		return (1);
	shell->str = readline(prompt);
	free(prompt);
	if (shell->str == NULL)
	{
		printf("exit\n");
		return (0);
	}
	add_history(shell->str);
	return (1);
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;
	int		loop;
	int		interactive;

	(void)ac;
	(void)av;
	loop = 1;
	interactive = isatty(STDIN_FILENO);
	shell = init_shell(envp);
	if (interactive)
		printf("%s", MINISHELL_BANNER);
	while (loop)
	{
		if (interactive)
			loop = ft_interactive(&shell);
		else
			loop = ft_non_interactive(&shell);
		if (!loop)
			break ;
		loop = shell_loop(&shell);
	}
	free_string_array(shell.envp);
	rl_clear_history();
	return (shell.exit_status);
}
