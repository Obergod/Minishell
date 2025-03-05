/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 18:49:35 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/05 19:00:43 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"
#include <stdio.h>
#include <string.h>

/*
	@brief Test du garbage collector
	Liste de toutes les fonctions du garbage collector

	gc_init() -> Initialise le garbage collector
	gc_alloc() -> Ajoute un pointeur au garbage collector
	gc_malloc() -> Alloue de la mémoire et l'enregistre dans le garbage collector
	gc_free() -> Libère une allocation du garbage collector
	gc_destroy() -> Détruit le garbage collector
	gc_contains() -> Vérifie si une allocation est présente dans le garbage collector
	gc_count() -> Compte le nombre d'allocations dans le garbage collector

*/

int main(void)
{
	t_gc_head *head;
	void *ptr;

	head = gc_init();
	ptr = strdup("Hello");
	gc_alloc(ptr, head);
	ptr = gc_malloc(sizeof(char) * 10, head);
	gc_free(ptr, head);
	gc_destroy(head);
	gc_count(head);
}
