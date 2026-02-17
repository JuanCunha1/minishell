/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 11:54:57 by jmarques          #+#    #+#             */
/*   Updated: 2026/02/17 10:41:47 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_pwd(char *key, char *pwd, char ***envp)
{
	int		i;
	char	*tmp;
	size_t	len;

	if (!pwd)
		return ;
	len = ft_strlen(key);
	i = 0;
	while ((*envp)[i])
	{
		if (!ft_strncmp((*envp)[i], key, len))
		{
			tmp = ft_strjoin(key, pwd);
			if (tmp)
			{
				free((*envp)[i]);
				(*envp)[i] = tmp;
			}
			break ;
		}
		i++;
	}
}

int	check_path(char *path)
{
	if (!path)
		return (1);
	if (chdir(path) == -1)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

char	*correct_env_value(char **args, char **envp)
{
	char	*path;

	if (!args[1] || !ft_strncmp(args[1], "~", 2))
	{
		path = get_env_value("HOME", envp);
		if (!path)
		{
			ft_putendl_fd("minishell: cd: HOME not set", 2);
			return (NULL);
		}
	}
	else if (!ft_strncmp(args[1], "-", 2))
	{
		path = get_env_value("OLDPWD", envp);
		if (!path)
		{
			ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
			return (NULL);
		}
		printf("%s\n", path);
	}
	else
		path = args[1];
	return (path);
}

int	ft_cd(char **args, char ***envp)
{
	char	*old_pwd;
	char	*path;
	char	*new_pwd;

	old_pwd = getcwd(NULL, 0);
	path = correct_env_value(args, *envp);
	if (!path || check_path(path))
	{
		free(old_pwd);
		return (1);
	}
	new_pwd = getcwd(NULL, 0);
	update_pwd("OLDPWD=", old_pwd, envp);
	update_pwd("PWD=", new_pwd, envp);
	free(new_pwd);
	free(old_pwd);
	return (0);
}
