/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 10:38:12 by jmarques          #+#    #+#             */
/*   Updated: 2025/12/02 10:38:14 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**get_path_dirs(char **env)
{
	const char	*name = "PATH";
	char		*env_path;
	int			i;
	size_t		len;

	len = ft_strlen(name);
	i = 0;
	env_path = NULL;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			env_path = (env[i] + len + 1);
		i++;
	}
	if (!env_path)
		return (NULL);
	return (ft_split(env_path, ':'));
}

static char	*join_path_and_cmd(const char *dir, const char *cmd)
{
	char	*with_slash;
	char	*full;

	if (*dir == '\0')
		return (ft_strdup(cmd));
	with_slash = ft_strjoin(dir, "/");
	if (!with_slash)
		return (NULL);
	full = ft_strjoin(with_slash, cmd);
	free(with_slash);
	return (full);
}

void	ft_free_tab(char **tab)
{
	size_t	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

static char	*find_executable(char *cmd, char **paths)
{
	char	*candidate;
	int		i;

	if (!cmd)
		return (NULL);
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		candidate = join_path_and_cmd(paths[i], cmd);
		if (!candidate)
		{
			i++;
			continue ;
		}
		if (access(candidate, X_OK) == 0)
			return (candidate);
		free(candidate);
		i++;
	}
	return (NULL);
}

char	*get_path(char *cmd, char **env)
{
	char	**path_dirs;
	char	*resolved_path;

	path_dirs = get_path_dirs(env);
	resolved_path = NULL;
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			resolved_path = ft_strdup(cmd);
	}
	else
		resolved_path = find_executable(cmd, path_dirs);
	ft_free_tab(path_dirs);
	return (resolved_path);
}
