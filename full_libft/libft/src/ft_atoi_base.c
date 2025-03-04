/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:49:09 by mafioron          #+#    #+#             */
/*   Updated: 2025/01/30 13:49:10 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_space(char c)
{
	if ((c == ' ') || (c >= 9 && c <= 13))
		return (1);
	else
		return (0);
}

static int	check_base(char *str)
{
	int	i;
	int	j;

	i = 0;
	if (str[0] == 0 || str[1] == '\0')
		return (0);
	while (str[i])
	{
		if (is_space(str[i]) == 1)
			return (0);
		if (str[i] == '-' || str[i] == '+')
			return (0);
		j = i + 1;
		while (str[j])
		{
			if (str[i] == str[j])
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

static int	is_inbase(char c, char *base_from)
{
	int	i;

	i = 0;
	while (base_from[i])
	{
		if (base_from[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

int	ft_atoi_base(char *nbr, char *base_from)
{
	int	i;
	int	res;
	int	neg;

	res = 0;
	neg = 1;
	i = 0;
	if (check_base(base_from) == 0)
		return (0);
	while (is_space(nbr[i]) == 1)
		i++;
	while (nbr[i] == '+' || nbr[i] == '-')
	{
		if (nbr[i] == '-')
			neg *= -1;
		i++;
	}
	while (is_inbase(nbr[i], base_from) != -1)
	{
		res *= ft_strlen(base_from);
		res += is_inbase(nbr[i], base_from);
		i++;
	}
	return (res * neg);
}
