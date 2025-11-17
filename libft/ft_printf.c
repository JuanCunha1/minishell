/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 13:58:45 by jmarques          #+#    #+#             */
/*   Updated: 2025/05/15 14:27:38 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
# include <stdarg.h>
#include "libft.h"

static void	check_type(const char input, va_list args, ssize_t	*count)
{
	if (input == 'c')
		print_char(va_arg(args, int), count);
	else if (input == 's')
		print_string(va_arg(args, char *), count);
	else if (input == 'p')
		print_pointer((unsigned long)va_arg(args, void *), count);
	else if (input == 'd' || input == 'i')
		putnbr_base_signed(va_arg(args, int), 10, INT_BASE, count);
	else if (input == 'u')
		putnbr_base(va_arg(args, unsigned int), 10, INT_BASE, count);
	else if (input == 'x')
		putnbr_base(va_arg(args, unsigned int), 16, HEX_LOWER, count);
	else if (input == 'X')
		putnbr_base(va_arg(args, unsigned int), 16, HEX_UPPER, count);
	else if (input == '%')
		print_char('%', count);
}

int	ft_printf(const char *input, ...)
{
	ssize_t		count;
	va_list		args;

	count = 0;
	va_start(args, input);
	while (*input)
	{
		if (*input == '%')
		{
			input++;
			if (*input)
				check_type(*input, args, &count);
			else
				break ;
		}
		else
			print_char(*input, &count);
		input++;
	}
	va_end(args);
	return ((int)count);
}
/*
#include <stdio.h>
int main()
{
	int *ptr = NULL;
	int count =0;
	char *ct = NULL;
	count =ft_printf("%c,%d,%s %i,%x %X %p\n",'a', 12, ct, 16,77,77,ptr);
	ft_printf("count = %d\n",count);
	count = printf("%c,%d,%s %i,%x %X %p\n",'a', 12, ct, 16,77,77,ptr);
	ft_printf("count = %d\n",count);
	int num = 42;
    ptr = &num;
    count = ft_printf("%c,%d,%s %i,%x %X %p\n",'a', 12, "deu", 16,77,77,ptr);
    ft_printf("count = %d\n",count);
	count = printf("%c,%d,%s %i,%x %X %p\n",'a', 12, "deu", 16,77,77,ptr);
	ft_printf("count = %d\n",count);
	return 0;
}
*/
