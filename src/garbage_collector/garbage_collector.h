/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:52:45 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/05 18:51:46 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GARBAGE_COLLECTOR_H
# define GARBAGE_COLLECTOR_H

# include <stdlib.h>

/**
 * @brief Structure pour stocker un pointeur et le chaîner dans la liste
 */
typedef struct s_gc
{
	void		*data;
	struct s_gc	*next;
}				t_gc;

/**
 * @brief Structure de tête du collecteur de mémoire
 */
typedef struct s_gc_head
{
	t_gc		*head;
}				t_gc_head;

/* Fonctions de base du collecteur */
t_gc_head		*gc_init(void);
int				gc_alloc(void *data, t_gc_head *head);
int				gc_free(void *ptr, t_gc_head *head);
void			gc_destroy(t_gc_head *head);

/* Fonctions utilitaires */
void			*gc_malloc(size_t size, t_gc_head *head);
int				gc_contains(void *ptr, t_gc_head *head);
size_t			gc_count(t_gc_head *head);

#endif
