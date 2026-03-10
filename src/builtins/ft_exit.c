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
	int		i;
	long	num;
	int		digit;

	if (!s || !*s)
		return (0);
	i = 0;
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (!ft_isdigit(s[i]))
		return (0);
	num = 0;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		digit = s[i] - '0';
		if (num > ((LONG_MAX - digit) / 10)
			&& num > ((LONG_MIN - digit) / 10))
			return (0);
		num = num * 10 + digit;
		i++;
	}
	return (1);
}

int	ft_exit(char **args)
{
	long	status;

	if (!args[1])
		return (-2);
	if (!is_numeric(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putendl_fd(" numeric argument required", 2);
		return (258);
	}
	if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		return (-1);
	}
	status = ft_atoi(args[1]);
	return ((unsigned char)status);
}
