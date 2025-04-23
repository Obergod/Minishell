/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:03:32 by mafioron          #+#    #+#             */
/*   Updated: 2025/04/23 18:03:34 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/garbage_collector.h"

static long	gc_abs(long n)
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
	n = gc_abs(n);
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

char	*gc_itoa(int nb, t_gc_head *gc)
{
	char	*res;
	int		len;
	int		i;
	long	n;

	n = nb;
	len = a_len(n);
	i = len - 1;
	res = gc_malloc(sizeof(char) * len + 1, gc);
	if (!res)
		return (NULL);
	res = to_str(n, res, len, i);
	return (res);
}
