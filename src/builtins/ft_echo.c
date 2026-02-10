/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rubmedin <rubmedin@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 15:45:31 by rubmedin          #+#    #+#             */
/*   Updated: 2025/12/04 15:46:00 by rubmedin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(char *args[], t_shell *shell)
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
	shell->exit_status = 0;
}
