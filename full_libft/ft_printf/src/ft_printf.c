/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 20:29:25 by mafioron          #+#    #+#             */
/*   Updated: 2024/11/18 17:43:27 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include "ft_printf.h"

int	print_cases(va_list vl, char c)
{
	int	put_len;

	put_len = 0;
	if (c == 'c')
		put_len = ft_printchar(va_arg(vl, int));
	else if (c == 's')
		put_len = ft_printstr(va_arg(vl, char *));
	else if (c == 'p')
		put_len = ft_printptr(va_arg(vl, size_t));
	else if (c == 'd' || c == 'i')
		put_len = ft_printnbs(va_arg(vl, int));
	else if (c == 'u')
		put_len = ft_printunsigned(va_arg(vl, unsigned int));
	else if (c == 'x' || c == 'X')
		put_len = ft_printnbr_base(va_arg(vl, unsigned int), c);
	else if (c == '%')
		put_len = ft_printchar('%');
	else
	{
		put_len += ft_printchar('%');
		put_len += ft_printchar(c);
	}
	return (put_len);
}

int	ft_printf(const char *str, ...)
{
	va_list	vl;
	char	*s;
	int		put_len;

	put_len = 0;
	s = (char *)str;
	va_start(vl, str);
	while (*s)
	{
		if (*s == '%')
		{
			put_len += print_cases(vl, *++s);
			s++;
		}
		else
		{
			put_len += ft_printchar(*s);
			s++;
		}
	}
	va_end(vl);
	return (put_len);
}
/*
#include <stdio.h>

int	main()
{
	int	i = 2147483647;
	char	c = 'c';
	char	*s = "HELLOWORLD";
	unsigned int u = 123;
	int	hex = 4234;
	void	*n = NULL;

//	printf("moi ; %d\n", ft_printf("%X\n", -42));
//	printf("printf : %d\n", printf("%X\n", -42));

	 printf("\nTaille TOI:%d\n\n",ft_printf("char %c, str %s,
	 ptr %p\n int, %d : %i, unsigned %u, hex %x %% %w\n", c, s, u, i, i, u, hex));
	 printf("\nTaille PRINTF:%d\n\n,",printf("char %c, str %s,
	 ptr %p\n int, %d : %i, unsigned %u, hex %x %% %w\n", c, s, u, i, i, u, hex));
}*/
