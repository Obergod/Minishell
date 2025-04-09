/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 17:06:08 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/05 18:38:59 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtin.h"
#include <stdio.h>

int main(int ac, char **av, char **envp)
{
	t_minishell minishell;
	t_gc_head *gc_head;

	(void)ac;
	// (void)av;
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
	// printf("---- Test de la commande ft_env ----\n");
	ft_env(&minishell);
	// printf("---- Fin du test, code de retour: %d ----\n", status);

	// printf("\n---- AVANT ft_export ----\n");
	// int i;
	// for (i = 0; av[i]; i++) {
	// 	printf("av[%d]: %s\n", i, av[i]);
	// }

	int status2 = ft_export(av + 1, &minishell);
	printf("---- Fin du test ft_unset, code de retour: %d ----\n", status2);

	printf("\n---- APRÈS ft_export - Nouvel environnement ----\n");
	// t_env *env = minishell.env;
	// while (env)
	// {
	// 	printf("%s\n", env->raw);
	// 	env = env->next;
	// }

	// Test de la commande ft_env
	// printf("---- Test de la commande ft_env ----\n");
	ft_env(&minishell);
	// printf("---- Fin du test, code de retour: %d ----\n", status2);
	//Test commande ft_echo
	// char *test[] = {"echo",s "salut", NULL};
	// printf("---- Test de la commande ft_echo ----\n");
	// int status = ft_echo(av + 1);
	// printf("---- Fin du test, code de retour: %d ----\n", status);

	//Test commande cd
	// printf("---- Test de la commande ft_cd ----\n");
	// int status = ft_cd(av + 1);
	// printf("---- Fin du test, code de retour: %d ----\n", status);

	// Libération de la mémoire
	gc_destroy(gc_head);
	return (0);
}
