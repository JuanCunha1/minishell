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
	if ((a[i] == '=' || a[i] == '\0') && (b[i] == '=' || b[i] == '\0'))
		return (0);
	if (a[i] == '=' || a[i] == '\0')
		return (-1);
	return (1);
}

int	is_valid_identifier(char *s)
{
	int	i;

	i = 0;
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

static void	sort_env(char **env)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (env[i])
	{
		j = i + 1;
		while (env[j])
		{
			if (cmp_env_name(env[i], env[j]) > 0)
			{
				tmp = env[i];
				env[i] = env[j];
				env[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	print_export_line(char *var)
{
	char	*eq;
	int		len;

	eq = ft_strchr(var, '=');
	if (!eq)
		len = ft_strlen(var);
	else
		len = eq - var;
	if (!eq || eq[1] == '\0')
		printf("declare -x %.*s\n", len, var);
	else
		printf("declare -x %.*s=\"%s\"\n", len, var, eq + 1);
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
	while (i < len)
	{
		print_export_line(copy[i++]);
	}
	free(copy);
	return (0);
}
