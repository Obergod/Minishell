/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 16:54:25 by mafioron          #+#    #+#             */
/*   Updated: 2024/11/08 17:46:51 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*res;
	char	*stock;
	int		len1;
	int		len2;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	res = malloc(sizeof(char) * (len1 + len2) + 1);
	if (!res)
		return (NULL);
	stock = res;
	while (len1--)
		*res++ = *s1++;
	while (len2--)
		*res++ = *s2++;
	*res = '\0';
	return (stock);
}
/*
int	main(int ac, char **av)
{
	printf("this is join : %s", ft_strjoin(av[1], av[2]));
}*/
