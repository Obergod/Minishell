/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:40:48 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/21 18:48:54 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"
#include "../../includes/token.h"
#include "../../includes/garbage_collector.h"
#include <stdio.h>

// Fonction pour débugger et afficher le contenu d'une liste chaînée de commandes
void debug_cmd_list(t_cmd *cmd)
{
	int cmd_count = 0;

	while (cmd)
	{
		cmd_count++;
		printf("\n---- Commande #%d ----\n", cmd_count);
		printf("Command raw: %s\n", cmd->command_raw ? cmd->command_raw : "NULL");

		printf("Arguments: ");
		if (cmd->command)
		{
			for (int i = 0; cmd->command[i] != NULL; i++)
			{
				printf("[%s] ", cmd->command[i]);
			}
		}
		else
		{
			printf("NULL");
		}
		printf("\n");

		printf("Arg count: %zu\n", cmd->_arg_count);
		printf("Arg capacity: %zu\n", cmd->_arg_capacity);
		printf("Infile: %s\n", cmd->infile ? cmd->infile : "NULL");
		printf("Outfile: %s\n", cmd->outfile ? cmd->outfile : "NULL");
		printf("Append: %d\n", cmd->append);

		printf("Logic operator type: ");
		switch (cmd->logic_operator_type)
		{
			case NONE: printf("NONE\n"); break;
			case AND: printf("AND\n"); break;
			case OR: printf("OR\n"); break;
			case OPEN_PARENTHESIS: printf("OPEN_PARENTHESIS\n"); break;
			case CLOSE_PARENTHESIS: printf("CLOSE_PARENTHESIS\n"); break;
			default: printf("UNKNOWN\n");
		}

		printf("Heredoc: %s\n", cmd->heredoc ? cmd->heredoc : "NULL");

		cmd = cmd->next;
	}
}

int main(int argc, char **argv)
{
	t_minishell minishell;
	t_token *tokens;
	t_cmd *commands;

	if (argc < 2)
	{
		printf("Usage: %s \"commande à parser\"\n", argv[0]);
		return 1;
	}

	// Initialisation du garbage collector
	minishell.gc = gc_init();
	minishell.env = NULL;
	minishell.cmd = NULL;

	// Tokenisation de l'argument
	tokens = tokenize(argv[1], &minishell);
	if (!tokens)
	{
		printf("Erreur: échec de la tokenisation\n");
		gc_destroy(minishell.gc);
		return 1;
	}

	// Affichage des tokens pour vérification
	printf("==== Tokens ====\n");
	t_token *current = tokens;
	int token_count = 0;
	while (current)
	{
		token_count++;
		printf("Token #%d: '%s' (Type: %d, State: %d)\n",
			token_count, current->str, current->type, current->state);
		current = current->next;
	}

	// Appel à la fonction parsing
	commands = parsing(tokens, &minishell);

	// Vérification du résultat
	if (!commands)
	{
		printf("\nLa fonction parsing a retourné NULL. Vérifiez les erreurs.\n");
		gc_destroy(minishell.gc);
		return 1;
	}

	// Affichage des commandes parsées
	printf("\n==== Commandes parsées ====\n");
	debug_cmd_list(commands);

	// Libération de la mémoire
	gc_destroy(minishell.gc);
	return 0;
}
