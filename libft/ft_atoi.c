/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:38:50 by jmarques          #+#    #+#             */
/*   Updated: 2025/05/05 14:25:24 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_atoi_helper(const char *nptr, int i, int is_negative)
{
	int	nbr;

	nbr = 0;
	while (nptr[i] && ft_isdigit(nptr[i]) == 1)
	{
		if (nbr > (INT_MAX - (nptr[i] - '0')) / 10)
		{
			if (is_negative == 1)
				return (INT_MIN + (nbr * 10 + (nptr[i] - '0') - INT_MAX) - 1);
			else
				return (INT_MAX - (nbr * 10 + (nptr[i] - '0') + INT_MIN) + 1);
		}
		nbr = (nbr * 10) + (nptr[i] - '0');
		i++;
	}
	return (nbr * is_negative);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	is_negative;

	is_negative = 1;
	i = 0;
	if (!nptr)
		return (0);
	while ((nptr[i] && (nptr[i] >= 9 && nptr[i] <= 13)) || nptr[i] == ' ')
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			is_negative = -1;
		i++;
	}
	return (ft_atoi_helper(nptr, i, is_negative));
}
/*

#include <stdio.h>
 #include <stdlib.h>
int	main(void)
{
	printf("%d\n",ft_atoi("2147483647" ));
	printf("%d\n",atoi("2147483647"));
	return (0);
}*/
