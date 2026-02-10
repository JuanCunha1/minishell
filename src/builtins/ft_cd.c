/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 11:54:57 by jmarques          #+#    #+#             */
/*   Updated: 2026/02/10 11:55:26 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void	update_pwd(char *old_pwd, char **envp)
{
	int		i;
	char	*new_pwd;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		free(old_pwd);
		return ;
	}
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PWD=", 4) == 0)
		{
			free(envp[i]);
			envp[i] = ft_strjoin("PWD=", new_pwd);
		}
		else if (ft_strncmp(envp[i], "OLDPWD=", 7) == 0)
		{
			free(envp[i]);
			envp[i] = ft_strjoin("OLDPWD=", old_pwd);
		}
		i++;
	}
	free(new_pwd);
}

static char	*get_cd_path(char **args, char **envp)
{
	char	*home;

	if (!args[1])
	{
		home = get_env_value("HOME", envp);
		if (!home)
		{
			ft_putendl_fd("cd: HOME not set", 2);
			return (NULL);
		}
		return (home);
	}
	return (args[1]);
}

int	ft_cd(char **args, char **envp)
{
	char	*old_pwd;
	char	*next_path;

	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		return (1);
	next_path = get_cd_path(args, envp);
	if (!next_path)
	{
		free(old_pwd);
		return (1);
	}
	if (chdir(next_path) == -1)
	{
		perror("cd");
		free(old_pwd);
		return (1);
	}
	update_pwd(old_pwd, envp);
	free(old_pwd);
	return (0);
}
