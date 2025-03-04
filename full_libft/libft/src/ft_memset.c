/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 11:36:01 by mafioron          #+#    #+#             */
/*   Updated: 2024/11/11 14:18:21 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*dst;

	dst = (unsigned char *)s;
	while (n-- > 0)
		*dst++ = c;
	return (s);
}
/*
int	main(int ac, char **av)
{
	char *str;

	str = (char *)ft_memset(atoi(av[1]), '-', 7);
	printf("fill of memset : %s\n", str);
}*/
