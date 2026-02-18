/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 12:59:48 by jmarques          #+#    #+#             */
/*   Updated: 2026/02/10 12:02:37 by jmarques         ###   ########.fr       */
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

int	ft_unset(char **args, char ***envp)
{
	int		i;
	char	*name;
	int		arg;

	if (!args || !envp || !*envp || !args[1])
		return (1);
	arg = 1;
	while (args[arg])
	{
		name = args[arg];
		i = 0;
		while ((*envp)[i])
		{
			if (unset_util(*envp, name, i) == 0)
				break ;
			i++;
		}
		arg++;
	}
	return (0);
}
