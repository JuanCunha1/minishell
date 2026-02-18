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

int	env_length(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
		i++;
	return (i);
}

static int	add_env_var(char ***envp, char *arg)
{
	int		len;
	char	**new_env;
	int		i;

	len = env_length(*envp);
	new_env = malloc(sizeof(char *) * (len + 2));
	if (!new_env)
		return (1);
	i = 0;
	while (i < len)
	{
		new_env[i] = (*envp)[i];
		i++;
	}
	new_env[len] = ft_strdup(arg);
	if (!new_env[len])
	{
		free(new_env);
		return (1);
	}
	new_env[len + 1] = NULL;
	free(*envp);
	*envp = new_env;
	return (0);
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

static int	is_valid_identifier(char *s)
{
	int		i;

	if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(s, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (0);
	}
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(s, 2);
			ft_putendl_fd("': not a valid identifier", 2);
			return (0);
		}
		i++;
	}
	return (1);
}

void	handle_export_arg(char ***envp, char *arg)
{
	int		idx;
	char	*name;
	char	*equal;

	if (!is_valid_identifier(arg))
		return ;
	equal = ft_strchr(arg, '=');
	if (!equal)
		return ;
	if (equal)
	{
		name = ft_substr(arg, 0, equal - arg);
		if (!name)
			return ;
		idx = get_env_index(*envp, name);
		if (idx != -1)
		{
			free((*envp)[idx]);
			(*envp)[idx] = ft_strdup(arg);
		}
		else
			add_env_var(envp, arg);
		free(name);
	}
}
