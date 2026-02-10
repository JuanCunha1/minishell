/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rubmedin <rubmedin@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 15:39:21 by rubmedin          #+#    #+#             */
/*   Updated: 2025/12/04 15:39:32 by rubmedin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(char *s)
{
	int	i;

	if (!s || !*s)
		return (0);
	i = 0;
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	exit_error(t_shell *shell)
{
	char	*str;

	str = shell->ast->args[1];
	if (!is_numeric(str))
	{
		printf("exit\n");
		printf("bash: exit: %s: numeric argument required\n", str);
		free_string_array(shell->envp);
		free_shell(shell);
		shell->exit_status = 2;
		exit(shell->exit_status);
	}
	if (shell->ast->args[2])
	{
		printf("exit: too many arguments\n");
		shell->exit_status = 1;
		return ;
	}
	shell->exit_status = (unsigned char)ft_atoi(str);
	printf("exit\n");
	free_string_array(shell->envp);
	free_shell(shell);
	exit(shell->exit_status);
}

void	ft_exit(t_shell *shell)
{
	if (!shell->ast || !shell->ast->args[1])
	{
		free_string_array(shell->envp);
		free_shell(shell);
		printf("exit\n");
		exit(shell->exit_status);
	}
	exit_error(shell);
}
