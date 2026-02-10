/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 12:59:48 by jmarques          #+#    #+#             */
/*   Updated: 2026/01/27 12:59:50 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset_util(char **envp, char *name, int i)
{
	int	len;

	len = ft_strlen(name);
	if (!ft_strncmp(envp[i], name, len)
		&& envp[i][len] == '=')
	{
		free(envp[i]);
		while (envp[i + 1])
		{
			envp[i] = envp[i + 1];
			i++;
		}
		envp[i] = NULL;
		return (0);
	}
	return (1);
}

void	ft_unset(t_shell *shell)
{
	int		i;
	char	*name;
	int		arg;

	if (!shell || !shell->envp || !shell->ast || !shell->ast->args[1])
		return ;
	arg = 1;
	while (shell->ast->args[arg])
	{
		name = shell->ast->args[arg];
		i = 0;
		while (shell->envp[i])
		{
			if (unset_util(shell->envp, name, i) == 0)
				break ;
			i++;
		}
		arg++;
	}
}
