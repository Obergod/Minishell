/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printnbr_base.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 19:30:35 by mafioron          #+#    #+#             */
/*   Updated: 2024/11/17 20:42:48 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printnbr_base(size_t n, char c)
{
	int		len;
	char	*low_base;
	char	*up_base;

	len = 0;
	low_base = "0123456789abcdef";
	up_base = "0123456789ABCDEF";
	if (n >= 16)
		len += ft_printnbr_base(n / 16, c);
	if (c == 'x')
		ft_printchar(low_base[n % 16]);
	if (c == 'X')
		ft_printchar(up_base[n % 16]);
	len++;
	return (len);
}

/*
#include <stdio.h>

int	main()
{
	printf("\nlen nb : %d\n", ft_printnbr_base(42, 'X'));
}*/
