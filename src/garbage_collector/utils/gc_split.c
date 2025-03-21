/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:10:03 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/21 16:04:38 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/garbage_collector.h"

/**
 * @brief Compte le nombre de mots dans une chaîne délimitée par un caractère
 *
 * @param str Chaîne à analyser
 * @param c Caractère délimiteur
 * @return int Nombre de mots
 */
int	count_split_words(const char *str, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*str)
	{
		if (*str != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*str == c)
			in_word = 0;
		str++;
	}
	return (count);
}

/**
 * @brief Calcule la longueur d'un mot délimité par un caractère
 *
 * @param str Chaîne contenant le mot
 * @param c Caractère délimiteur
 * @return int Longueur du mot
 */
static int	word_len(const char *str, char c)
{
	int	len;

	len = 0;
	while (str[len] && str[len] != c)
		len++;
	return (len);
}

/**
 * @brief Extrait un mot d'une chaîne et le duplique avec le garbage collector
 *
 * @param str Chaîne contenant le mot
 * @param c Caractère délimiteur
 * @param gc Structure de tête du garbage collector
 * @return char* Mot dupliqué
 */
static char	*get_word(const char *str, char c, t_gc_head *gc)
{
	int		len;
	char	*word;
	int		i;

	len = word_len(str, c);
	word = gc_malloc(sizeof(char) * (len + 1), gc);
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

/**
 * @brief Divise une chaîne en un tableau de sous-chaînes, en utilisant le
 * garbage collector pour la gestion de la mémoire
 *
 * @param str Chaîne à diviser
 * @param c Caractère délimiteur
 * @param gc Structure de tête du garbage collector
 * @return char** Tableau de chaînes terminé par NULL
 */
char	**gc_split(const char *str, char c, t_gc_head *gc)
{
	char	**result;
	int		words;
	int		i;

	if (!str)
		return (NULL);
	words = count_split_words(str, c);
	result = gc_malloc(sizeof(char *) * (words + 1), gc);
	if (!result)
		return (NULL);
	i = 0;
	while (i < words)
	{
		while (*str && *str == c)
			str++;
		result[i] = get_word(str, c, gc);
		if (!result[i])
			return (NULL);
		str += word_len(str, c);
		i++;
	}
	result[i] = NULL;
	return (result);
}
