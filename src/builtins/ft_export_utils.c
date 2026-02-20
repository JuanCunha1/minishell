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

static void	swap(char **a, char **b)
{
	char	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static int	cmp_env_name(char *a, char *b)
{
	int	i;

	i = 0;
	while (a[i] && a[i] != '=' && b[i] && b[i] != '=')
	{
		if (a[i] != b[i])
			return ((unsigned char)a[i] - (unsigned char)b[i]);
		i++;
	}
	if ((a[i] == '=' || !a[i]) && (b[i] == '=' || !b[i]))
		return (0);
	if (a[i] == '=' || !a[i])
		return (-1);
	return (1);
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
			if (cmp_env_name(env[i], env[j]) > 0)
				swap(&env[i], &env[j]);
			j++;
		}
		i++;
	}
}

static void	print_export_line(char *var)
{
	char	*equal;

	equal = ft_strchr(var, '=');
	if (!equal)
	{
		printf("declare -x %s\n", var);
		return ;
	}
	printf("declare -x %.*s=\"%s\"\n",
		(int)(equal - var), var, equal + 1);
}

int	print_export_sorted(char **envp)
{
	char	**copy;
	int		len;
	int		i;

	len = env_length(envp);
	copy = malloc(sizeof(char *) * (len + 1));
	if (!copy)
		return (1);
	i = 0;
	while (i < len)
	{
		copy[i] = envp[i];
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
