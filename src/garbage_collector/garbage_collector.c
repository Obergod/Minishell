/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:52:33 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/21 16:12:37 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/garbage_collector.h"

/**
 * @brief Initialise un nouveau collecteur de mémoire
 *
 * @return t_gc_head* Pointeur vers la structure de tête ou NULL en cas d'erreur
 */
t_gc_head	*gc_init(void)
{
	t_gc_head	*head;

	head = malloc(sizeof(t_gc_head));
	if (!head)
		return (NULL);
	head->head = NULL;
	return (head);
}

/**
 * @brief Ajoute un pointeur à gérer par le collecteur
 *
 * @param data Pointeur vers les données allouées
 * @param head Pointeur vers la structure de tête du collecteur
 * @return int 0 en cas de succès, 1 en cas d'erreur
 */
int	gc_alloc(void *data, t_gc_head *head)
{
	t_gc	*node;

	if (!head || !data)
		return (1);
	if (gc_contains(data, head) == 1)
		return (1);
	node = malloc(sizeof(t_gc));
	if (!node)
		return (1);
	node->data = data;
	node->next = head->head;
	head->head = node;
	return (0);
}

/**
 * @brief Supprime une allocation spécifique du collecteur
 *
 * @param ptr Pointeur vers les données à libérer
 * @param head Pointeur vers la structure de tête du collecteur
 * @return int 0 en cas de succès,
	1 si le pointeur n'est pas trouvé ou en cas d'erreur
 */
int	gc_free(void *ptr, t_gc_head *head)
{
	t_gc	*tmp;
	t_gc	*prev;

	if (!head || !ptr)
		return (1);
	tmp = head->head;
	prev = NULL;
	while (tmp)
	{
		if (tmp->data == ptr)
		{
			if (prev)
				prev->next = tmp->next;
			else
				head->head = tmp->next;
			free(tmp->data);
			free(tmp);
			return (0);
		}
		prev = tmp;
		tmp = tmp->next;
	}
	return (1);
}

/**
 * @brief Détruit le collecteur et libère toutes les allocations
 *
 * @param head Pointeur vers la structure de tête du collecteur
 */
void	gc_destroy(t_gc_head *head)
{
	t_gc	*current;
	t_gc	*next;

	if (!head)
		return ;
	current = head->head;
	while (current)
	{
		next = current->next;
		if (current->data != NULL)
			free(current->data);
		free(current);
		current = next;
	}
	free(head);
}
