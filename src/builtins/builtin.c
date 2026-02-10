/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 12:20:47 by jmarques          #+#    #+#             */
/*   Updated: 2026/01/27 12:20:49 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	builtin(t_ast *ast, t_shell *sh, char *str)
{
	if (ft_strncmp(str, "cd", 3) == 0)
		ft_cd(ast, sh);
	if (ft_strncmp(str, "echo", 5) == 0)
		ft_echo(ast->args, sh);
	if (ft_strncmp(str, "pwd", 4) == 0)
		ft_pwd(sh);
	if (ft_strncmp(str, "exit", 5) == 0)
		ft_exit(sh);
	if (ft_strncmp(str, "unset", 6) == 0)
		ft_unset(sh);
	if (ft_strncmp(str, "export", 7) == 0)
		ft_export(sh);
	if (ft_strncmp(str, "env", 4) == 0)
		ft_env(sh);
}
