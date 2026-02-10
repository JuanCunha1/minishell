/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 12:01:56 by jmarques          #+#    #+#             */
/*   Updated: 2026/02/10 12:02:01 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strndup(const char *s, size_t n)
{
	size_t	i;
	char	*dup;

	if (!s)
		return (NULL);
	dup = (char *)malloc(sizeof(char) * (n + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (i < n && s[i])
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

static char	*get_name(char *arg, int *has_equal)
{
	char	*eq;
	char	*name;

	eq = ft_strchr(arg, '=');
	if (eq)
	{
		*has_equal = 1;
		name = ft_strndup(arg, eq - arg);
	}
	else
	{
		*has_equal = 0;
		name = ft_strdup(arg);
	}
	if (!name)
		perror("strdup failed");
	return (name);
}

int	ft_export(char **args, char **envp)
{
	int		i;
	char	*name;
	int		has_equal;

	if (!args || !envp || !args[1])
		return (1);
	i = 0;
	while (args[++i])
	{
		name = get_name(args[i], &has_equal);
		if (!name)
			continue ;
		handle_export_arg(envp, name, has_equal, args[i]);
		free(name);
	}
	return (0);
}
