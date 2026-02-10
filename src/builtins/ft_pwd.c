/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 13:47:08 by jmarques          #+#    #+#             */
/*   Updated: 2025/11/27 13:56:32 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd_error(t_shell *sh)
{
	if (sh->ast->args[1] && sh->ast->args[1][0] == '-'
		&& sh->ast->args[1][1])
	{
		printf("bash: pwd: %s: bad option\n", sh->ast->args[1]);
		printf("pwd: usage: pwd\n");
		sh->exit_status = 2;
		return (-1);
	}
	return (0);
}

void	ft_pwd(t_shell *shell)
{
	char	*pwd;

	shell->exit_status = 0;
	if (pwd_error(shell) == -1)
		return ;
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		perror("pwd");
		shell->exit_status = 1;
		return ;
	}
	ft_putendl_fd(pwd, 1);
	free(pwd);
	pwd = NULL;
}
