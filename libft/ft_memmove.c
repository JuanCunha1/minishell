/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:29:17 by jmarques          #+#    #+#             */
/*   Updated: 2025/04/30 15:55:17 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	memmove_forward(char *strdest, const char *strsrc, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		strdest[i] = strsrc[i];
		i++;
	}
}

static void	memmove_backward(char *strdest, const char *strsrc, size_t n)
{
	size_t	i;

	i = n;
	while (i > 0)
	{
		strdest[i - 1] = strsrc[i - 1];
		i--;
	}
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char		*strdest;
	const char	*strsrc;

	if ((dest == NULL && src == NULL) || n == 0)
		return (dest);
	strdest = (char *)dest;
	strsrc = (const char *)src;
	if (strdest < strsrc || strdest >= (strsrc + n))
		memmove_forward(strdest, strsrc, n);
	else
		memmove_backward(strdest, strsrc, n);
	return (dest);
}
