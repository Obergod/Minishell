/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 16:06:55 by mafioron          #+#    #+#             */
/*   Updated: 2024/11/14 17:55:07 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *str, unsigned int start, size_t len)
{
	char	*s;
	char	*stock;

	if (start >= ft_strlen(str))
	{
		s = malloc(sizeof(char));
		if (!s)
			return (NULL);
		*s = '\0';
		return (s);
	}
	if (ft_strlen(str) - start < len)
		s = malloc(sizeof(char) * (ft_strlen(str) - start + 1));
	else if (len > ft_strlen(str))
		s = malloc(sizeof(char) * ft_strlen(str) + 1);
	else
		s = malloc(sizeof(char) * len + 1);
	if (!s)
		return (NULL);
	stock = s;
	str += start;
	while (len-- && *str)
		*s++ = *str++;
	*s = '\0';
	return (stock);
}
/*
int	main(int ac, char **av)
{
	printf("this is substr : %s\n", ft_substr(av[1], atoi(av[2]), atoi(av[3])));
}*/
