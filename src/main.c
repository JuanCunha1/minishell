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

int	shell_loop(t_shell *sh)
{
	sh->tokens = NULL;
	sh->ast = NULL;
	set_signals_prompt();
	sh->str = readline("minishell> ");
	if (sh->str == NULL)
	{
		printf("exit\n");
		return (0);
	}
	if (sh->str[0] == '\0')
	{
		free(sh->str);
		sh->str = NULL;
		return (1);
	}
	add_history(sh->str);
	if (lexer(sh) == -1)
		return (1);
	else if (parser(sh) == 1)
		execute_ast(sh->ast, sh);
	g_signal = 0;
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
	return (g_signal);
}
/*		readline()
 *	-------------------
 *	La funcion readline(const char *str) es la que te lee
 *  el stdin(salida estandar del input) y te lo guarda en str.
 *	Uso readline porque la otra opcion seria read y usar
 *  read seria poner mas trabajo porque read tiene
 *	la limitacion de leer y readline tine ya incorporado
 *  como un historial de comandos y
 *	no necesita buffer size si no que pilla todo el comando.
 *
 *		add_history()
 *	-------------------
 *	El add_history sirve para el tema de agregar un mini
 *  historial de comandos.
 *
 * */
