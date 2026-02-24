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

void	multiple_line(t_shell *sh, char **lines)
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

static char	*shorten_home(char *pwd, char *home)
{
	char	*tmp;

	if (!pwd || !home)
		return (ft_strdup(pwd));
	if (!ft_strncmp(pwd, home, ft_strlen(home)))
	{
		tmp = ft_strjoin("~", pwd + ft_strlen(home));
		if (!tmp)
			return (NULL);
		return (tmp);
	}
	return (ft_strdup(pwd));
}

static char	*write_pwd(char **envp)
{
	char	*pwd;
	char	*home;
	char	*short_pwd;
	char	*tmp;
	char	*prompt;

	pwd = get_env_value("PWD", envp);
	home = get_env_value("HOME", envp);
	if (!pwd)
		pwd = "";
	short_pwd = shorten_home(pwd, home);
	if (!short_pwd)
		return (NULL);
	tmp = ft_strjoin("\001\033[1;32m\002minishell:\001\033[0m\002:"\
"\001\033[1;34m\002", short_pwd);
	free(short_pwd);
	if (!tmp)
		return (NULL);
	prompt = ft_strjoin(tmp, "\001\033[0m\002$ ");
	free(tmp);
	return (prompt);
}

int	shell_loop(t_shell *sh)
{
	char	**lines;

	set_signals_prompt();
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
	char	*prompt;
	int		loop;

	(void)ac;
	(void)av;
	loop = 1;
	shell = init_shell(envp);
	printf("%s", MINISHELL_BANNER);
	while (loop)
	{
		prompt = write_pwd(shell.envp);
		if (!prompt)
			continue ;
		shell.str = readline(prompt);
		free(prompt);
		loop = shell_loop(&shell);
	}
	free_string_array(shell.envp);
	rl_clear_history();
	return (shell.exit_status);
}
