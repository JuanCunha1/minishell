/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 12:02:07 by jmarques          #+#    #+#             */
/*   Updated: 2026/02/10 12:02:09 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_length(char **env)
{
	int	i;

	if (!env)
		return (0);
	i = 0;
	while (env[i])
		i++;
	return (i);
}

static void	append_env_var(char **envp, char *arg)
{
	int		len;
	char	**tmp;

	len = env_length(envp);
	tmp = (char **)realloc(envp, sizeof(char *) * (len + 2));
	if (!tmp)
	{
		perror("realloc failed");
		return ;
	}
	envp = tmp;
	envp[len] = ft_strdup(arg);
	if (!envp[len])
	{
		perror("strdup failed");
		envp[len] = NULL;
		return ;
	}
	envp[len + 1] = NULL;
}

static int	get_env_index(char **envp, char *name)
{
	int		i;
	int		len;

	len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], name, len)
			&& envp[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static int	is_valid_env_name(char *s)
{
	int		i;

	if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	handle_export_arg(char **envp, char *name, int has_equal, char *arg)
{
	int	idx;

	if (!is_valid_env_name(name))
	{
		ft_putstr_fd("export: not a valid identifier", 2);
		ft_putendl_fd(name, 2);
		return ;
	}
	idx = get_env_index(envp, name);
	if (!has_equal)
		return ;
	if (idx != -1)
	{
		free(envp[idx]);
		if (has_equal)
			envp[idx] = ft_strdup(arg);
		else
			envp[idx] = ft_strdup(envp[idx]);
		if (!envp[idx])
			perror("strdup failed");
		return ;
	}
	else
		append_env_var(envp, arg);
}
