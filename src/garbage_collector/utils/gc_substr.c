/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:20:05 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/21 16:12:37 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/garbage_collector.h"

/**
 * @brief Extrait une sous-chaîne d'une chaîne en utilisant le garbage collector
 *
 * @param s Chaîne source
 * @param start Index de début de la sous-chaîne
 * @param len Longueur maximale de la sous-chaîne
 * @param gc Structure de tête du garbage collector
 * @return char* Sous-chaîne extraite et allouée
 */
char	*gc_substr(char const *s, unsigned int start, size_t len, t_gc_head *gc)
{
	char	*result;
	size_t	s_len;
	size_t	i;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (gc_strdup("", gc));
	if (len > s_len - start)
		len = s_len - start;
	result = gc_malloc(sizeof(char) * (len + 1), gc);
	if (!result)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		result[i] = s[start + i];
		i++;
	}
	result[i] = '\0';
	return (result);
}
