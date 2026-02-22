/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 12:01:56 by jmarques          #+#    #+#             */
/*   Updated: 2026/02/17 10:43:16 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_name(const char *s)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	return (ft_substr(s, 0, i));
}

static int	add_env_var(char ***envp, char *arg)
{
	char	**new_env;
	int		len;
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

static int	update_env_var(char ***envp, char *arg)
{
	char	*name;
	int		idx;
	char	*dup;

	name = get_name(arg);
	if (!name)
		return (1);
	idx = get_env_index(*envp, name);
	free(name);
	if (idx == -1)
		return (add_env_var(envp, arg));
	dup = ft_strdup(arg);
	if (!dup)
		return (1);
	free((*envp)[idx]);
	(*envp)[idx] = dup;
	return (0);
}

static int new_env(char *arg, char ***envp)
{
	int		idx;
	char	*tmp;

	idx = get_env_index(*envp, arg);
	if (idx == -1)
	{
		tmp = ft_strjoin(arg, "=");
		if (!tmp || update_env_var(envp, tmp))
			return (1);
		free(tmp);
	}
	return (0);
}

int	ft_export(char **args, char ***envp)
{
	int		i;
	int		status;

	status = 0;
	if (!args[1])
		return (print_export_sorted(*envp));
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			status = 1;
		}
		else if (ft_strchr(args[i], '='))
			status = update_env_var(envp, args[i]);
		else
			status = new_env(args[i], envp);
		i++;
	}
	return (status);
}
