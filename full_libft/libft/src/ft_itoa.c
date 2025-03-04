/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 13:27:16 by mafioron          #+#    #+#             */
/*   Updated: 2024/11/14 19:18:25 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static long	ft_abs(long n)
{
	if (n < 0)
		return (-n);
	return (n);
}

static int	a_len(long n)
{
	int	count;

	count = 0;
	if (n <= 0)
		count++;
	n = ft_abs(n);
	while (n > 0)
	{
		count++;
		n /= 10;
	}
	return (count);
}

char	*to_str(long n, char *res, int len, int i)
{
	if (n < 0)
	{
		res[0] = '-';
		n = -n;
	}
	if (n == 0)
		res[0] = '0';
	while (n > 0)
	{
		res[i] = (n % 10) + '0';
		n /= 10;
		i--;
	}
	res[len] = '\0';
	return (res);
}

char	*ft_itoa(int nb)
{
	char	*res;
	int		len;
	int		i;
	long	n;

	n = nb;
	len = a_len(n);
	i = len - 1;
	res = malloc(sizeof(char) * len + 1);
	if (!res)
		return (NULL);
	res = to_str(n, res, len, i);
	return (res);
}
