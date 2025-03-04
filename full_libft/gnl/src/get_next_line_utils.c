/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <mafioron@student.42.fr>          +#+  +:+       +#+       */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 18:22:50 by mafioron          #+#    #+#            */
/*   Updated: 2024/12/03 18:22:53 by mafioron         ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	gnl_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	*gnl_calloc(size_t nmemb, size_t size)
{
	void	*res;
	char	*r;
	size_t	len;

	len = (size * nmemb);
	res = (void *)malloc(len);
	r = (char *)res;
	if (!res)
		return (NULL);
	while (len--)
		*r++ = '\0';
	return (res);
}

char	*gnl_strdup(const char *s)
{
	int		i;
	char	*dst;
	size_t	len;

	i = 0;
	len = gnl_strlen(s);
	dst = malloc(sizeof(char) * len + 1);
	if (!dst)
		return (NULL);
	while (s[i])
	{
		dst[i] = s[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

char	*gnl_strjoin(char const *s1, char const *s2)
{
	char	*res;
	char	*stock;
	int		len1;
	int		len2;

	len1 = gnl_strlen(s1);
	len2 = gnl_strlen(s2);
	res = (char *)gnl_calloc((len1 + len2) + 1, sizeof(char));
	if (!res)
		return (NULL);
	stock = res;
	while (len1--)
		*res++ = *s1++;
	while (len2--)
		*res++ = *s2++;
	return (stock);
}

int	modif_strchr(const char *s, int c)
{
	if (!s)
		return (0);
	while (*s)
	{
		if (*s == (unsigned char)c)
		{
			return (1);
		}
		s++;
	}
	return (0);
}
