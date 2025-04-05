/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 18:00:00 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/04 17:01:37 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include "includes/env_parsing.h"
#include "includes/builtin.h"
#include "includes/garbage_collector.h"
#include <stdio.h>

int main(int ac, char **av, char **envp)
{
	t_minishell minishell;
	t_gc_head *gc_head;

	(void)ac;
	(void)av;

	// Initialisation du garbage collector
	gc_head = gc_init();
	minishell.gc = gc_head;

	// Initialisation de l'environnement
	minishell.env = env_parsing(envp, &minishell);
	if (!minishell.env)
	{
		printf("Erreur: Échec de l'analyse de l'environnement\n");
		gc_destroy(gc_head);
		return (1);
	}

	// Test de la commande ft_env
	printf("---- Test de la commande ft_env ----\n");
	int status = ft_env(&minishell);
	printf("---- Fin du test, code de retour: %d ----\n", status);

	// Libération de la mémoire
	gc_destroy(gc_head);
	return (0);
}
