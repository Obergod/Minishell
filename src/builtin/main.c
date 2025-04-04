/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 17:06:08 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/04 19:34:50 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtin.h"
#include <stdio.h>

int main(int ac, char **av, char **envp)
{
	t_minishell minishell;
	t_gc_head *gc_head;

	(void)ac;
	(void)av;
	// char **test = { NULL };
	// envp = test;

	// Initialisation du garbage collector
	gc_head = gc_init();
	if (!gc_head)
	{
		printf("Erreur: Échec de l'initialisation du garbage collector\n");
		return (1);
	}
	minishell.gc = gc_head;

	// Initialisation de l'environnement
	minishell.env = env_parsing(envp, &minishell);
	if (!minishell.env)
	{
		minishell.env = init_env(&minishell);
		t_env *env = minishell.env;
		while (env)
		{
			printf("%s\n", env->raw);
			env = env->next;
		}
	}

	// Test de la commande ft_env
	printf("---- Test de la commande ft_env ----\n");
	int status = ft_env(&minishell);
	printf("---- Fin du test, code de retour: %d ----\n", status);

	// Libération de la mémoire
	gc_destroy(gc_head);
	return (0);
}
