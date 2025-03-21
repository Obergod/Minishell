/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_strndup_array.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:07:24 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/21 16:07:48 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/garbage_collector.h"

char **gc_strndup_array(char **s1, size_t size, t_gc_head *gc)
{
	int i;
	int j;
	char *new_str;
	char **new_array;

	j = 0;
	new_array = gc_malloc((sizeof(char *) * size) + 1, gc);
	while (s1[j])
	{
		new_str = gc_malloc((sizeof(char) * ft_strlen(s1[j]) + 1), gc);
		if (!new_str)
			return (NULL);
		i = 0;
		while (s1[j][i])
		{
			new_str[i] = s1[j][i];
			i++;
		}
		new_str[i] = '\0';
		new_array[j] = new_str;
		j++;
	}
	new_array[j] = NULL;
	return (new_array);
}
