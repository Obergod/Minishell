/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:15:02 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/21 16:12:37 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/garbage_collector.h"

/**
 * @brief Concatène deux chaînes de caractères en utilisant le garbage collector
 *
 * @param s1 Première chaîne
 * @param s2 Deuxième chaîne
 * @param gc Structure de tête du garbage collector
 * @return char* Nouvelle chaîne allouée avec les deux chaînes concaténées
 */
char	*gc_strjoin(char *s1, char *s2, t_gc_head *gc)
{
	char	*result;
	int		i;
	int		j;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (gc_strdup(s2, gc));
	if (!s2)
		return (gc_strdup(s1, gc));
	result = gc_malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1), gc);
	if (!result)
		return (NULL);
	i = -1;
	j = -1;
	while (s1[++i])
		result[i] = s1[i];
	while (s2[++j])
		result[i + j] = s2[j];
	result[i + j] = '\0';
	return (result);
}
