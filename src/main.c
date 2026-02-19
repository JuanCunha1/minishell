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

void multiple_line(t_shell *sh, char **lines)
{
	int	i;

	i = 0;
	while (lines[i])
	{
		sh->tokens = lexer(lines[i], sh->envp, sh->exit_status);
		if (!sh->tokens)
		{
			i++;
			continue;
		}
		if (check_tokens(sh->tokens) == 2)
		{
			sh->exit_status = 2;
			i++;
			continue;
		}
		sh->ast = parser(sh);
		set_signals_parent_exec();
		sh->exit_status = execute_ast(sh->ast, &sh->envp);
		free_shell(sh);
		i++;
	}
}

int	shell_loop(t_shell *sh)
{
	char	**lines;

	set_signals_prompt();
	sh->str = readline("minishell> ");
	if (g_signal == 130)
		sh->exit_status = 130;
	if (sh->str == NULL)
	{
		printf("exit\n");
		return (0);
	}
	if (sh->str[0] == '\0')
		return (1);
	add_history(sh->str);
	lines = ft_split(sh->str, '\n');
	free(sh->str);
	sh->str = NULL;
	if (!lines)
		return (1);
	multiple_line(sh, lines);
	free_string_array(lines);
	return (1);
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;

	(void)ac;
	(void)av;
	shell = init_shell(envp);
	while (shell_loop(&shell))
		free_shell(&shell);
	free_string_array(shell.envp);
	rl_clear_history();
	return (shell.exit_status);
}
