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

int	pwd_error(char **args)
{
	if (args[1] && args[1][0] == '-'
		&& args[1][1])
	{
		printf("bash: pwd: %s: bad option\n", args[1]);
		printf("pwd: usage: pwd\n");
		return (2);
	}
	return (0);
}

int	ft_pwd(char **args)
{
	char	*pwd;

	if (pwd_error(args) == 2)
		return (2);
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		perror("pwd");
		return (1);
	}
	ft_putendl_fd(pwd, 1);
	free(pwd);
	pwd = NULL;
	return (0);
}
