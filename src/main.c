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

int	g_signal = 0;

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

int	shell_loop(t_shell *sh)
{
	set_signals_prompt();
	sh->str = readline("minishell> ");
	if (sh->str == NULL)
	{
		printf("exit\n");
		return (0);
	}
	if (sh->str[0] == '\0')
		return (1);
	add_history(sh->str);
	sh->tokens = lexer(sh->str, sh->envp, sh->exit_status);
	if (!sh->tokens)
		return (1);
	if (check_tokens(sh->tokens) == 2)
	{
		sh->exit_status = 2;
		return (1);
	}
	sh->ast = parser(sh);
	sh->exit_status = execute_ast(sh->ast, &sh->envp, 0);
	return (1);
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;

	if (ac > 1 && av[1])
		return (0);
	shell = init_shell(envp);
	while (shell_loop(&shell))
		free_shell(&shell);
	free_string_array(shell.envp);
	rl_clear_history();
	return (shell.exit_status);
}
