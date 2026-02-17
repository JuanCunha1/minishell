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

static void	swap(char **a, char **b)
{
	char	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	sort_env(char **env)
{
	int	i;
	int	j;

	i = 0;
	while (env[i])
	{
		j = i + 1;
		while (env[j])
		{
			if (ft_strncmp(env[i], env[j], ft_strlen(env[i])
					+ ft_strlen(env[j])) > 0)
				swap(&env[i], &env[j]);
			j++;
		}
		i++;
	}
}

static void	print_export_line(char *var)
{
	char	*equal;
	int		name_len;

	equal = ft_strchr(var, '=');
	if (!equal)
	{
		printf("declare -x %s\n", var);
		return ;
	}
	name_len = equal - var;
	ft_putstr_fd("declare -x ", 1);
	write(1, var, name_len);
	ft_putstr_fd("=\"", 1);
	ft_putstr_fd(equal + 1, 1);
	ft_putendl_fd("\"", 1);
}

int	print_export_sorted(char ***envp)
{
	char	**copy;
	int		len;
	int		i;

	len = env_length(*envp);
	copy = malloc(sizeof(char *) * (len + 1));
	if (!copy)
		return (1);
	i = 0;
	while (i < len)
	{
		copy[i] = (*envp)[i];
		i++;
	}
	copy[len] = NULL;
	sort_env(copy);
	i = 0;
	while (copy[i])
	{
		print_export_line(copy[i]);
		i++;
	}
	free(copy);
	return (0);
}

int	ft_export(char **args, char ***envp)
{
	int		i;

	if (!args[1])
		return (print_export_sorted(envp));
	i = 1;
	while (args[i])
	{
		handle_export_arg(envp, args[i]);
		i++;
	}
	return (0);
}
