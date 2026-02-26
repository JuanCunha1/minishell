/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 12:20:47 by jmarques          #+#    #+#             */
/*   Updated: 2026/02/10 11:53:57 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin_parent(char **args, char ***envp)
{
	int	return_status;

	if (ft_strncmp(args[0], "exit", 5) == 0)
	{
		printf("exit\n");
		return_status = ft_exit(args);
		if (return_status == -2)
			return (-1);
		if (return_status != -1)
			exit(return_status);
		return (2);
	}
	return (builtin(args, envp));
}

int	is_builtin(char *str)
{
	if (!str)
		return (0);
	if (ft_strncmp(str, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(str, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(str, "exit", 5) == 0)
		return (1);
	if (ft_strncmp(str, "export", 7) == 0)
		return (1);
	if (ft_strncmp(str, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(str, "env", 4) == 0)
		return (1);
	if (ft_strncmp(str, "unset", 6) == 0)
		return (1);
	return (0);
}

int	builtin(char **args, char ***envp)
{
	if (ft_strncmp(args[0], "cd", 3) == 0)
		return (ft_cd(args, envp));
	else if (ft_strncmp(args[0], "echo", 5) == 0)
		return (ft_echo(args));
	else if (ft_strncmp(args[0], "pwd", 4) == 0)
		return (ft_pwd());
	else if (ft_strncmp(args[0], "exit", 5) == 0)
		return (ft_exit(args));
	else if (ft_strncmp(args[0], "unset", 6) == 0)
		return (ft_unset(args, envp));
	else if (ft_strncmp(args[0], "export", 7) == 0)
		return (ft_export(args, envp));
	else if (ft_strncmp(args[0], "env", 4) == 0)
		return (ft_env(args, *envp));
	return (1);
}
