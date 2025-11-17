/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmarques <jmarques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 13:58:45 by jmarques          #+#    #+#             */
/*   Updated: 2025/05/20 14:28:41 by jmarques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>

# define HEX_LOWER	"0123456789abcdef"
# define HEX_UPPER	"0123456789ABCDEF"
# define INT_BASE	"0123456789"

int		ft_printf(const char *input, ...);
void	putnbr_base(unsigned long n, int base, const char *digits,
			ssize_t *count);
void	putnbr_base_signed(long int n, int base, const char *digits,
			ssize_t *count);
void	print_pointer(unsigned long value, ssize_t *count);
void	print_string(char *s, ssize_t *count);
void	print_char(char c, ssize_t *count);

#endif
