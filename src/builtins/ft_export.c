/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rubmedin <rubmedin@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 20:45:50 by rubmedin          #+#    #+#             */
/*   Updated: 2025/12/07 20:59:42 by rubmedin         ###   ########.fr       */
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

void	ft_export(t_shell *shell)
{
	int		i;
	char	*name;
	int		has_equal;
	char	**args;

	if (!shell || !shell->envp || !shell->ast
		|| !shell->ast->args || !shell->ast->args[1])
		return ;
	i = 0;
	args = shell->ast->args;
	while (args[++i])
	{
		name = get_name(args[i], &has_equal);
		if (!name)
			continue ;
		handle_export_arg(shell, name, has_equal, args[i]);
		free(name);
	}
}
