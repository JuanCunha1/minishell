/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 11:56:32 by jmarques          #+#    #+#             */
/*   Updated: 2026/02/10 11:56:37 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(char *s)
{
	int	i;

	if (!s || !*s)
		return (0);
	i = 0;
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	exit_check(char **args)
{
	char	*str;

	str = args[1];
	if (!is_numeric(str))
	{
		printf("exit\n");
		printf("bash: exit: %s: numeric argument required\n", str);
		exit(2);
	}
	if (args[2])
	{
		printf("bash: exit: too many arguments\n");
		return (0);
	}
	printf("exit\n");
	exit(ft_atoi(str));
	return (1);
}

int	ft_exit(char **args)
{
	if (!args[1])
	{
		printf("exit\n");
		exit(0);
	}
	if (!exit_check(args))
		return (2);
	return (1);
}
