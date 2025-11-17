/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 12:25:53 by jmarques          #+#    #+#             */
/*   Updated: 2025/05/20 12:25:56 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	putnbr_base_signed(long n, int base, const char *digits, ssize_t *count)
{
	unsigned long	un;

	if (n < 0)
	{
		print_char('-', count);
		un = (unsigned long)(-n);
	}
	else
		un = (unsigned long)n;
	putnbr_base(un, base, digits, count);
}

void	putnbr_base(unsigned long n, int base, const char *digits,
	ssize_t *count)
{
	if (n >= (unsigned long)base)
		putnbr_base(n / base, base, digits, count);
	print_char(digits[n % base], count);
}

void	print_pointer(unsigned long ptr, ssize_t *count)
{
	if (!ptr)
	{
		print_string("(nil)", count);
		return ;
	}
	print_string("0x", count);
	putnbr_base(ptr, 16, HEX_LOWER, count);
}

void	print_char(char c, ssize_t *count)
{
	*count += write(1, &c, 1);
}

void	print_string(char *s, ssize_t *count)
{
	if (!s)
		s = "(null)";
	while (*s)
	{
		print_char(*s, count);
		s++;
	}
}
