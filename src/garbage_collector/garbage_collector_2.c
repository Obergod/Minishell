/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 18:49:10 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/21 15:23:10 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/garbage_collector.h"

/**
 * @brief Alloue de la mémoire et l'enregistre dans le collecteur
 *
 * @param size Taille en octets à allouer
 * @param head Pointeur vers la structure de tête du collecteur
 * @return void* Pointeur vers la mémoire allouée ou NULL en cas d'erreur
 */
void	*gc_malloc(size_t size, t_gc_head *head)
{
	void	*data;

	if (!head || size == 0)
		return (NULL);
	data = malloc(size);
	if (!data)
		return (NULL);
	if (gc_alloc(data, head))
	{
		free(data);
		return (NULL);
	}
	return (data);
}

/**
 * @brief Vérifie si un pointeur est géré par le collecteur
 *
 * @param ptr Pointeur à vérifier
 * @param head Pointeur vers la structure de tête du collecteur
 * @return int 1 si le pointeur est géré, 0 sinon
 */
int	gc_contains(void *ptr, t_gc_head *head)
{
	t_gc	*current;

	if (!head || !ptr)
		return (0);
	current = head->head;
	while (current)
	{
		if (current->data == ptr)
			return (1);
		current = current->next;
	}
	return (0);
}

/**
 * @brief Compte le nombre d'allocations gérées par le collecteur
 *
 * @param head Pointeur vers la structure de tête du collecteur
 * @return size_t Nombre d'allocations
 */
size_t	gc_count(t_gc_head *head)
{
	t_gc	*current;
	size_t	count;

	if (!head)
		return (0);
	count = 0;
	current = head->head;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

void	gc_print(t_gc_head *head)
{
	t_gc	*current;

	if (!head)
		return ;
	current = head->head;
	while (current)
	{
		printf("%p\n", current->data);
		current = current->next;
	}
}
