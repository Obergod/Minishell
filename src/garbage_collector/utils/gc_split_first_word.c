/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_split_first_word.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:22:31 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/21 16:16:33 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/garbage_collector.h"

static char	*get_next_word(const char *str, char c, int *pos, t_gc_head *gc)
{
	int		len;
	char	*word;
	int		i;

	len = 0;
	while (str[*pos + len] && str[*pos + len] != c)
		len++;
	word = gc_malloc(sizeof(char) * (len + 1), gc);
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = str[*pos + i];
		i++;
	}
	word[i] = '\0';
	*pos += len;
	return (word);
}

static char	*get_rest_of_string(const char *str, int pos, t_gc_head *gc)
{
	int		len;
	char	*word;
	int		i;

	len = 0;
	while (str[pos + len])
		len++;
	word = gc_malloc(sizeof(char) * (len + 1), gc);
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = str[pos + i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	**gc_split_first_word(const char *str, char c, t_gc_head *gc)
{
	char	**result;
	int		pos;

	if (!str)
		return (NULL);
	result = gc_malloc(sizeof(char *) * 3, gc);
	if (!result)
		return (NULL);
	pos = 0;
	result[0] = get_next_word(str, c, &pos, gc);
	if (str[pos] == c)
	{
		pos++;
		result[1] = get_rest_of_string(str, pos, gc);
	}
	else
	{
		result[1] = gc_malloc(sizeof(char), gc);
		if (result[1])
			result[1][0] = '\0';
	}
	result[2] = NULL;
	return (result);
}
