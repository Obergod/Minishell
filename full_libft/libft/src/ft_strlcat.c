/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 18:53:38 by mafioron          #+#    #+#             */
/*   Updated: 2024/11/06 18:54:44 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	int		i;
	int		j;
	size_t	len;

	len = ft_strlen(dest);
	i = 0;
	j = len;
	if (len >= size)
		return (ft_strlen(src) + size);
	while (src[i] && (size_t)i < (size - len - 1))
	{
		dest[j + i] = src[i];
		i++;
	}
	dest[j + i] = '\0';
	return (ft_strlen(src) + len);
}
