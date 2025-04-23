/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:52:45 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/24 16:17:15 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GARBAGE_COLLECTOR_H
# define GARBAGE_COLLECTOR_H

# include <stdlib.h>
# include <stdio.h>
# include "minishell.h"

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

/**
 * @brief Structure pour stocker un pointeur et le chaîner dans la liste
 */
typedef struct s_gc
{
	void		*data;
	struct s_gc	*next;
}				t_gc;

/**
 * @brief Structure de tête du garbage collector
 */
typedef struct s_gc_head
{
	t_gc		*head;
}				t_gc_head;

/* Fonctions de base du garbage collector */
t_gc_head		*gc_init(void);
int				gc_alloc(void *data, t_gc_head *head);
int				gc_free(void *ptr, t_gc_head *head);
void			gc_destroy(t_gc_head *head);
void			*gc_malloc(size_t size, t_gc_head *head);
int				gc_contains(void *ptr, t_gc_head *head);
size_t			gc_count(t_gc_head *head);
void			gc_print(t_gc_head *head);

/* Fonctions utilitaires du garbage collector */
char			*gc_strdup(const char *s1, t_gc_head *gc);
char			*gc_strndup(const char *s, size_t n, t_gc_head *gc);
char			*gc_strjoin(char *s1, char *s2, t_gc_head *gc);
char			*gc_strjoin_three(char *s1, char *s2, char *s3, t_gc_head *gc);
char			**gc_split(const char *str, char c, t_gc_head *gc);
char			**gc_split_first_word(const char *str, char c, t_gc_head *gc);
char			*gc_substr(char const *s, unsigned int start, size_t len, t_gc_head *gc);
char			**gc_strndup_array(char **s1, size_t size, t_gc_head *gc);
void			*gc_calloc(size_t count, size_t size, t_gc_head *gc);
char			**gc_strdup_array(char **s1, t_gc_head *gc);
char			*gc_itoa(int nb, t_gc_head *gc);

#endif
