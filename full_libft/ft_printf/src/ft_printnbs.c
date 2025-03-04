/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printnbs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:46:14 by mafioron          #+#    #+#             */
/*   Updated: 2024/11/18 15:50:22 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printnbs(int nb)
{
	long	n;
	int		len;

	len = 0;
	n = nb;
	if (n < 0)
	{
		ft_printchar('-');
		n = -n;
		len++;
	}
	if (n >= 10)
		len += ft_printnbs(n / 10);
	ft_printchar(n % 10 + '0');
	len++;
	return (len);
}
