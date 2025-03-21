/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_strjoin_three.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:13:03 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/21 16:09:54 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/garbage_collector.h"

char *gc_strjoin_three(char *s1, char *s2, char *s3, t_gc_head *gc)
{
	char *result;
	int i;
	int j;
	int k;

	result = gc_malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + 1), gc);
	if (!result)
		return (NULL);
	i = -1;
	j = -1;
	k = -1;
	while (s1[++i])
		result[i] = s1[i];
	while (s2[++j])
		result[i + j] = s2[j];
	while (s3[++k])
		result[i + j + k] = s3[k];
	result[i + j + k] = '\0';
	return (result);
}
