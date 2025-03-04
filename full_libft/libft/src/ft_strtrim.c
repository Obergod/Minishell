/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 17:50:45 by mafioron          #+#    #+#             */
/*   Updated: 2024/11/14 14:54:36 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_charset(char c, char const *set)
{
	while (*set)
		if (*set++ == c)
			return (1);
	return (0);
}

static int	get_trimlen(char const *s1, char const *set)
{
	int	s;
	int	e;

	s = 0;
	while (s1[s])
		s++;
	e = s - 1;
	while (is_charset(s1[e], set) == 1)
		e--;
	s = 0;
	while (is_charset(s1[s], set))
		s++;
	return (e - s + 1);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*res;
	char	*stock;
	int		len;

	len = get_trimlen(s1, set);
	if (len >= 0)
		res = malloc(sizeof(char) * len + 1);
	else
	{
		res = malloc(sizeof(char));
		*res = '\0';
		return (res);
	}
	if (!res)
		return (NULL);
	while (*s1 && is_charset(*s1, set))
		s1++;
	stock = res;
	while (len--)
		*res++ = *s1++;
	*res = '\0';
	return (stock);
}
/*
int	main(int ac, char **av)
{
	printf("this is trimmed : %s\n", ft_strtrim(av[1], av[2]));
}*/
