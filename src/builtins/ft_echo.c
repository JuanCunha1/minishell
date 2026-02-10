/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 11:55:41 by jmarques          #+#    #+#             */
/*   Updated: 2026/02/10 11:55:54 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(char **args)
{
	int	i;
	int	j;

	i = 1;
	if (args[1] && !ft_strncmp(args[1], "-n", 3))
		i++;
	while (args[i])
	{
		j = 0;
		while (args[i][j])
		{
			ft_putchar_fd(args[i][j], 1);
			if (!args[i][j + 1])
				ft_putchar_fd(32, 1);
			j++;
		}
		i++;
	}
	if (!(args[1] && !ft_strncmp(args[1], "-n", 3)))
		ft_putchar_fd(10, 1);
	return (0);
}
