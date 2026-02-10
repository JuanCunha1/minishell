/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rubmedin <rubmedin@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 15:39:21 by rubmedin          #+#    #+#             */
/*   Updated: 2025/12/04 15:39:32 by rubmedin         ###   ########.fr       */
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

static void	exit_check(char **args)
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
		return ;
	}
	printf("exit\n");
	exit(ft_atoi(str));
}

int	ft_exit(char **args)
{
	if (args[1])
	{
		printf("exit\n");
		exit(0);
	}
	exit_check(args);
	return (1);
}
