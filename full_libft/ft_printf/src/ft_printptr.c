/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printptr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 20:03:00 by mafioron          #+#    #+#             */
/*   Updated: 2024/11/18 17:43:17 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include "ft_printf.h"

int	ft_putptr(size_t n)
{
	char		*low_base;
	int			len;

	len = 0;
	low_base = "0123456789abcdef";
	if (n >= 16)
		len += ft_putptr(n / 16);
	ft_printchar(low_base[n % 16]);
	len++;
	return (len);
}

int	ft_printptr(size_t n)
{
	int	len;

	len = 0;
	if (!n)
		return (ft_printstr("(nil)"));
	len += ft_printstr("0x");
	len += ft_putptr(n);
	return (len);
}
