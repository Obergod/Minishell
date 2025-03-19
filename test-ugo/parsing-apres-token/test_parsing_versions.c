/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parsing_versions.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:00:00 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/19 18:53:29 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include <stdio.h>

// Version avec couleurs pour l'utilisateur humain
void test_parsing_colorized(t_minishell *minishell)
{
	t_token *tokens = NULL;
	t_cmd *result;
	char *cyan = "\033[0;36m";   // Cyan pour les descriptions de test
	char *yellow = "\033[0;33m";  // Jaune pour les avertissements
	char *red = "\033[0;31m";     // Rouge pour les erreurs
	char *reset = "\033[0m";     // Réinitialisation de la couleur

	// Test 1: Commande simple
	printf("\n%s=== Test 1: Commande simple ===\n", cyan);
	printf("Commande: ls -la%s\n", reset);
	add_token(&tokens, create_token("ls", T_WORD, minishell));
	add_token(&tokens, create_token("-la", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd(result, 1);
	else
		printf("%sLe parsing a retourné NULL (erreur détectée)%s\n", red, reset);

	// Réinitialiser pour le prochain test
	tokens = NULL;

	// Test 2: Commande avec redirection entrée
	printf("\n%s=== Test 2: Commande avec redirection entrée ===\n", cyan);
	printf("Commande: cat < input.txt%s\n", reset);
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("input.txt", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd(result, 2);
	else
		printf("%sLe parsing a retourné NULL (erreur détectée)%s\n", red, reset);

	// Réinitialiser pour le prochain test
	tokens = NULL;

	// Test 3: Commande avec redirection sortie
	printf("\n%s=== Test 3: Commande avec redirection sortie ===\n", cyan);
	printf("Commande: echo hello > output.txt%s\n", reset);
	add_token(&tokens, create_token("echo", T_WORD, minishell));
	add_token(&tokens, create_token("hello", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("output.txt", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd(result, 3);
	else
		printf("%sLe parsing a retourné NULL (erreur détectée)%s\n", red, reset);

	// Réinitialiser pour le prochain test
	tokens = NULL;

	// Test 4: Commande avec pipe
	printf("\n%s=== Test 4: Commande avec pipe ===\n", cyan);
	printf("Commande: ls -l | grep test%s\n", reset);
	add_token(&tokens, create_token("ls", T_WORD, minishell));
	add_token(&tokens, create_token("-l", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("grep", T_WORD, minishell));
	add_token(&tokens, create_token("test", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
	{
		t_cmd *current = result;
		int cmd_num = 1;
		while (current)
		{
			printf("%sCommande %d:%s\n", cyan, cmd_num++, reset);
			print_cmd(current, 4);
			current = current->next;
		}
	}
	else
		printf("%sLe parsing a retourné NULL (erreur détectée)%s\n", red, reset);

	// Réinitialiser pour le prochain test
	tokens = NULL;

	// Test 5: Commande complexe avec plusieurs redirections et pipes
	printf("\n%s=== Test 5: Commande complexe ===\n", cyan);
	printf("Commande: cat < input.txt | grep pattern | wc -l >> result.txt%s\n", reset);
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("input.txt", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("grep", T_WORD, minishell));
	add_token(&tokens, create_token("-e", T_WORD, minishell));
	add_token(&tokens, create_token("pattern", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("wc", T_WORD, minishell));
	add_token(&tokens, create_token("-l", T_WORD, minishell));
	add_token(&tokens, create_token(">>", T_REDIR, minishell));
	add_token(&tokens, create_token("result.txt", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
	{
		t_cmd *current = result;
		int cmd_num = 1;
		while (current)
		{
			printf("%sCommande %d:%s\n", cyan, cmd_num++, reset);
			print_cmd(current, 5);
			current = current->next;
		}
	}
	else
		printf("%sLe parsing a retourné NULL (erreur détectée)%s\n", red, reset);

	// Tests avec cas d'erreur
	printf("\n%s========= TESTS AVEC CAS D'ERREUR =========%s\n\n", red, reset);

	// Test 6: Redirection sans fichier
	tokens = NULL;
	printf("\n%s=== Test 6: Redirection sans fichier ===\n", cyan);
	printf("%sCommande: cat < %s\n", reset, red);
	printf("%s(Ce test devrait générer une erreur car il manque le fichier après <)%s\n", yellow, reset);
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd(result, 0);
	else
		printf("%sLe parsing a retourné NULL (erreur détectée)%s\n", red, reset);

	// Test 7: Pipe sans commande après
	tokens = NULL;
	printf("\n%s=== Test 7: Pipe sans commande après ===\n", cyan);
	printf("%sCommande: ls | %s\n", reset, red);
	printf("%s(Ce test devrait générer une erreur car il manque une commande après |)%s\n", yellow, reset);
	add_token(&tokens, create_token("ls", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));

	result = parsing(tokens, minishell);
	if (result)
	{
		t_cmd *current = result;
		int cmd_num = 1;
		while (current)
		{
			printf("%sCommande %d:%s\n", cyan, cmd_num++, reset);
			print_cmd(current, 0);
			current = current->next;
		}
	}
	else
		printf("%sLe parsing a retourné NULL (erreur détectée)%s\n", red, reset);

	// Test 8: Pipe sans commande avant
	tokens = NULL;
	printf("\n%s=== Test 8: Pipe sans commande avant ===\n", cyan);
	printf("%sCommande: | ls%s\n", red, reset);
	printf("%s(Ce test devrait générer une erreur car il manque une commande avant |)%s\n", yellow, reset);
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("ls", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
	{
		t_cmd *current = result;
		int cmd_num = 1;
		while (current)
		{
			printf("%sCommande %d:%s\n", cyan, cmd_num++, reset);
			print_cmd(current, 0);
			current = current->next;
		}
	}
	else
		printf("%sLe parsing a retourné NULL (erreur détectée)%s\n", red, reset);

	// Test 9: Multiples redirections du même type
	tokens = NULL;
	printf("\n%s=== Test 9: Multiples redirections du même type ===\n", cyan);
	printf("%sCommande: cat < file1.txt < file2.txt%s\n", cyan, reset);
	printf("%s(Ce test vérifie comment votre code gère plusieurs redirections du même type - seule la dernière devrait être prise en compte)%s\n", yellow, reset);
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("file1.txt", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("file2.txt", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd(result, 0);
	else
		printf("%sLe parsing a retourné NULL (erreur détectée)%s\n", red, reset);

	// Test 10: Redirections multiples de différents types
	tokens = NULL;
	printf("\n%s=== Test 10: Redirections multiples de différents types ===\n", cyan);
	printf("%sCommande: cat < input.txt > output.txt >> append.txt << EOF%s\n", cyan, reset);
	printf("%s(Ce test vérifie comment votre code gère plusieurs redirections de types différents)%s\n", yellow, reset);
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("input.txt", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("output.txt", T_WORD, minishell));
	add_token(&tokens, create_token(">>", T_REDIR, minishell));
	add_token(&tokens, create_token("append.txt", T_WORD, minishell));
	add_token(&tokens, create_token("<<", T_REDIR, minishell));
	add_token(&tokens, create_token("EOF", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd(result, 0);
	else
		printf("%sLe parsing a retourné NULL (erreur détectée)%s\n", red, reset);
}

// Version simplifiée pour les LLM
void print_cmd_simple(t_cmd *cmd)
{
	int i;

	printf("Command Raw: %s\n", cmd->command_raw ? cmd->command_raw : "NULL");

	printf("Command Args: ");
	if (cmd->command)
	{
		i = 0;
		while (cmd->command[i])
		{
			printf("[%s] ", cmd->command[i]);
			i++;
		}
	}
	else
		printf("NULL");
	printf("\n");

	printf("Infile: %s\n", cmd->infile ? cmd->infile : "NULL");
	printf("Outfile: %s\n", cmd->outfile ? cmd->outfile : "NULL");
	printf("Append: %d\n", cmd->append);
	printf("Heredoc: %s\n", cmd->heredoc ? cmd->heredoc : "NULL");
	printf("-------------------\n");
}

void test_parsing_simple(t_minishell *minishell)
{
	t_token *tokens = NULL;
	t_cmd *result;
	t_cmd *current;
	int cmd_num;

	printf("=== TESTS DE BASE ===\n");

	// Test 1: Commande simple
	printf("\nTest 1: Commande simple\n");
	printf("Commande: ls -la\n");
	add_token(&tokens, create_token("ls", T_WORD, minishell));
	add_token(&tokens, create_token("-la", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd_simple(result);
	else
		printf("Le parsing a retourné NULL\n");

	// Test 2: Commande avec redirection entrée
	tokens = NULL;
	printf("\nTest 2: Commande avec redirection entrée\n");
	printf("Commande: cat < input.txt\n");
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("input.txt", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd_simple(result);
	else
		printf("Le parsing a retourné NULL\n");

	// Test 3: Commande avec redirection sortie
	tokens = NULL;
	printf("\nTest 3: Commande avec redirection sortie\n");
	printf("Commande: echo hello > output.txt\n");
	add_token(&tokens, create_token("echo", T_WORD, minishell));
	add_token(&tokens, create_token("hello", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("output.txt", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd_simple(result);
	else
		printf("Le parsing a retourné NULL\n");

	// Test 4: Commande avec pipe
	tokens = NULL;
	printf("\nTest 4: Commande avec pipe\n");
	printf("Commande: ls -l | grep test\n");
	add_token(&tokens, create_token("ls", T_WORD, minishell));
	add_token(&tokens, create_token("-l", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("grep", T_WORD, minishell));
	add_token(&tokens, create_token("test", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
	{
		current = result;
		cmd_num = 1;
		while (current)
		{
			printf("Commande %d:\n", cmd_num++);
			print_cmd_simple(current);
			current = current->next;
		}
	}
	else
		printf("Le parsing a retourné NULL\n");

	// Test 5: Commande complexe avec plusieurs redirections et pipes
	tokens = NULL;
	printf("\nTest 5: Commande complexe\n");
	printf("Commande: cat < input.txt | grep pattern | wc -l >> result.txt\n");
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("input.txt", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("grep", T_WORD, minishell));
	add_token(&tokens, create_token("pattern", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("wc", T_WORD, minishell));
	add_token(&tokens, create_token("-l", T_WORD, minishell));
	add_token(&tokens, create_token(">>", T_REDIR, minishell));
	add_token(&tokens, create_token("result.txt", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
	{
		current = result;
		cmd_num = 1;
		while (current)
		{
			printf("Commande %d:\n", cmd_num++);
			print_cmd_simple(current);
			current = current->next;
		}
	}
	else
		printf("Le parsing a retourné NULL\n");

	printf("\n=== TESTS CAS D'ERREUR ===\n");

	// Test 6: Redirection sans fichier
	tokens = NULL;
	printf("\nTest 6: Redirection sans fichier\n");
	printf("Commande: cat <\n");
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd_simple(result);
	else
		printf("Le parsing a retourné NULL (erreur détectée)\n");

	// Test 7: Pipe sans commande après
	tokens = NULL;
	printf("\nTest 7: Pipe sans commande après\n");
	printf("Commande: ls |\n");
	add_token(&tokens, create_token("ls", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));

	result = parsing(tokens, minishell);
	if (result)
	{
		current = result;
		cmd_num = 1;
		while (current)
		{
			printf("Commande %d:\n", cmd_num++);
			print_cmd_simple(current);
			current = current->next;
		}
	}
	else
		printf("Le parsing a retourné NULL (erreur détectée)\n");

	// Test 8: Multiples redirections
	tokens = NULL;
	printf("\nTest 8: Multiples redirections\n");
	printf("Commande: cat < file1.txt < file2.txt > out1.txt > out2.txt\n");
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("file1.txt", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("file2.txt", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("out1.txt", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("out2.txt", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd_simple(result);
	else
		printf("Le parsing a retourné NULL (erreur détectée)\n");

	// Test 9: Complexe avec heredoc
	tokens = NULL;
	printf("\nTest 9: Complexe avec heredoc\n");
	printf("Commande: cat << EOF | grep pattern >> result.txt\n");
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("<<", T_REDIR, minishell));
	add_token(&tokens, create_token("EOF", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("grep", T_WORD, minishell));
	add_token(&tokens, create_token("pattern", T_WORD, minishell));
	add_token(&tokens, create_token(">>", T_REDIR, minishell));
	add_token(&tokens, create_token("result.txt", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
	{
		current = result;
		cmd_num = 1;
		while (current)
		{
			printf("Commande %d:\n", cmd_num++);
			print_cmd_simple(current);
			current = current->next;
		}
	}
	else
		printf("Le parsing a retourné NULL (erreur détectée)\n");

	// Test 10: Redirection avant commande
	tokens = NULL;
	printf("\nTest 10: Redirection avant commande\n");
	printf("Commande: < input.txt cat\n");
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("input.txt", T_WORD, minishell));
	add_token(&tokens, create_token("cat", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd_simple(result);
	else
		printf("Le parsing a retourné NULL (erreur détectée)\n");

	// NOUVEAUX TESTS SUPPLÉMENTAIRES
	printf("\n=== TESTS SUPPLÉMENTAIRES ===\n");

	// Test 11: Heredoc avec pipe et grep
	tokens = NULL;
	printf("\nTest 11: Heredoc avec pipe et grep\n");
	printf("Commande: cat << EOF | grep \"hello there\"\n");
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("<<", T_REDIR, minishell));
	add_token(&tokens, create_token("EOF", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("grep", T_WORD, minishell));
	add_token(&tokens, create_token("hello there", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
	{
		current = result;
		cmd_num = 1;
		while (current)
		{
			printf("Commande %d:\n", cmd_num++);
			print_cmd_simple(current);
			current = current->next;
		}
	}
	else
		printf("Le parsing a retourné NULL (erreur détectée)\n");

	// Test 12: Redirections multiples d'entrée et sortie
	tokens = NULL;
	printf("\nTest 12: Redirections multiples d'entrée et sortie\n");
	printf("Commande: cat -e < input1.txt < input2.txt >> output.txt\n");
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("-e", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("input1.txt", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("input2.txt", T_WORD, minishell));
	add_token(&tokens, create_token(">>", T_REDIR, minishell));
	add_token(&tokens, create_token("output.txt", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd_simple(result);
	else
		printf("Le parsing a retourné NULL (erreur détectée)\n");

	// Test 13: Pipeline complexe à quatre commandes
	tokens = NULL;
	printf("\nTest 13: Pipeline complexe à quatre commandes\n");
	printf("Commande: ls -la | grep minishell | sort | uniq | wc -l\n");
	add_token(&tokens, create_token("ls", T_WORD, minishell));
	add_token(&tokens, create_token("-la", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("grep", T_WORD, minishell));
	add_token(&tokens, create_token("minishell", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("sort", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("uniq", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("wc", T_WORD, minishell));
	add_token(&tokens, create_token("-l", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
	{
		current = result;
		cmd_num = 1;
		while (current)
		{
			printf("Commande %d:\n", cmd_num++);
			print_cmd_simple(current);
			current = current->next;
		}
	}
	else
		printf("Le parsing a retourné NULL (erreur détectée)\n");

	// Test 14: Redirection avant la commande (grep)
	tokens = NULL;
	printf("\nTest 14: Redirection avant la commande (grep)\n");
	printf("Commande: < input.txt grep -e UN MOT TRES TRES TRES LONG asdaisdj asdioasjd asdoiasj dasoid234234 2342 234sfdf\n");
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("input.txt", T_WORD, minishell));
	add_token(&tokens, create_token("grep", T_WORD, minishell));
	add_token(&tokens, create_token("-e", T_WORD, minishell));
	add_token(&tokens, create_token("UN MOT TRES TRES TRES LONG asdaisdj asdioasjd asdoiasj dasoid234234 2342 234sfd3485237562347863428563473734263477777&@#*^@#!^@$$^@#@#@#$@$f\0", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd_simple(result);
	else
		printf("Le parsing a retourné NULL (erreur détectée)\n");

	// Test 15: Guillemets mixtes et noms de fichiers avec espaces
	tokens = NULL;
	printf("\nTest 15: Guillemets mixtes et noms de fichiers avec espaces\n");
	printf("Commande: echo \"hello 'buddy' \" < 'in file.txt' | cat -e >> \"out file.txt\"\n");
	add_token(&tokens, create_token("echo", T_WORD, minishell));
	add_token(&tokens, create_token("hello 'buddy' ", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("in file.txt", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("-e", T_WORD, minishell));
	add_token(&tokens, create_token(">>", T_REDIR, minishell));
	add_token(&tokens, create_token("out file.txt", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
	{
		current = result;
		cmd_num = 1;
		while (current)
		{
			printf("Commande %d:\n", cmd_num++);
			print_cmd_simple(current);
			current = current->next;
		}
	}
	else
		printf("Le parsing a retourné NULL (erreur détectée)\n");

	// Test 16: Variables d'environnement
	tokens = NULL;
	printf("\nTest 16: Variables d'environnement\n");
	printf("Commande: echo $USER$HOME/test\n");
	add_token(&tokens, create_token("echo", T_WORD, minishell));
	add_token(&tokens, create_token("$USER$HOME/test", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd_simple(result);
	else
		printf("Le parsing a retourné NULL (erreur détectée)\n");

	// Test 17: Guillemets non fermés
	tokens = NULL;
	printf("\nTest 17: Guillemets non fermés\n");
	printf("Commande: echo \"This is not closed\n");
	add_token(&tokens, create_token("echo", T_WORD, minishell));
	add_token(&tokens, create_token("This is not closed", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd_simple(result);
	else
		printf("Le parsing a retourné NULL (erreur détectée)\n");

	// Test 18: Redirection sans fichier (autre syntaxe)
	tokens = NULL;
	printf("\nTest 18: Redirection sans fichier (autre syntaxe)\n");
	printf("Commande: cat >\n");
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd_simple(result);
	else
		printf("Le parsing a retourné NULL (erreur détectée)\n");

	// Test 19: Opérateurs logiques (OR)
	tokens = NULL;
	printf("\nTest 19: Opérateurs logiques (OR)\n");
	printf("Commande: ls || grep something\n");
	add_token(&tokens, create_token("ls", T_WORD, minishell));
	add_token(&tokens, create_token("||", T_LOGIC, minishell));
	add_token(&tokens, create_token("grep", T_WORD, minishell));
	add_token(&tokens, create_token("something", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
	{
		current = result;
		cmd_num = 1;
		while (current)
		{
			printf("Commande %d:\n", cmd_num++);
			print_cmd_simple(current);
			current = current->next;
		}
	}
	else
		printf("Le parsing a retourné NULL (erreur détectée)\n");

	// Test 20: Pipes consécutifs
	tokens = NULL;
	printf("\nTest 20: Pipes consécutifs\n");
	printf("Commande: ls | | grep test\n");
	add_token(&tokens, create_token("ls", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("grep", T_WORD, minishell));
	add_token(&tokens, create_token("test", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
	{
		current = result;
		cmd_num = 1;
		while (current)
		{
			printf("Commande %d:\n", cmd_num++);
			print_cmd_simple(current);
			current = current->next;
		}
	}
	else
		printf("Le parsing a retourné NULL (erreur détectée)\n");

	// Test 21: Heredoc avec pipe et redirection
	tokens = NULL;
	printf("\nTest 21: Heredoc avec pipe et redirection\n");
	printf("Commande: cat <<EOF | wc -l > out.txt\n");
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("<<", T_REDIR, minishell));
	add_token(&tokens, create_token("EOF", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("wc", T_WORD, minishell));
	add_token(&tokens, create_token("-l", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("out.txt", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
	{
		current = result;
		cmd_num = 1;
		while (current)
		{
			printf("Commande %d:\n", cmd_num++);
			print_cmd_simple(current);
			current = current->next;
		}
	}
	else
		printf("Le parsing a retourné NULL (erreur détectée)\n");

	// Test 22: Guillemets échappés et noms de fichiers avec espaces
	tokens = NULL;
	printf("\nTest 22: Guillemets échappés et noms de fichiers avec espaces\n");
	printf("Commande: grep \\\"hello world\\\" < \\\"input file.txt\\\" >> \\\"my output file.txt\"\n");
	add_token(&tokens, create_token("grep", T_WORD, minishell));
	add_token(&tokens, create_token("\"hello world\"", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("\"input file.txt\"", T_WORD, minishell));
	add_token(&tokens, create_token(">>", T_REDIR, minishell));
	add_token(&tokens, create_token("\"my output file.txt\"", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd_simple(result);
	else
		printf("Le parsing a retourné NULL (erreur détectée)\n");

	// Test 23: Guillemets vides et espaces
	tokens = NULL;
	printf("\nTest 23: Guillemets vides et espaces\n");
	printf("Commande: echo \"\" \"\" \"test\" '' \" \"\n");
	add_token(&tokens, create_token("echo", T_WORD, minishell));
	add_token(&tokens, create_token("", T_WORD, minishell));
	add_token(&tokens, create_token("", T_WORD, minishell));
	add_token(&tokens, create_token("test", T_WORD, minishell));
	add_token(&tokens, create_token("", T_WORD, minishell));
	add_token(&tokens, create_token(" ", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd_simple(result);
	else
		printf("Le parsing a retourné NULL (erreur détectée)\n");

	// Test 24: Redirections multiples de sortie
	tokens = NULL;
	printf("\nTest 24: Redirections multiples de sortie\n");
	printf("Commande: echo coucou > test1.txt >> test2.txt\n");
	add_token(&tokens, create_token("echo", T_WORD, minishell));
	add_token(&tokens, create_token("coucou", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("test1.txt", T_WORD, minishell));
	add_token(&tokens, create_token(">>", T_REDIR, minishell));
	add_token(&tokens, create_token("test2.txt", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd_simple(result);
	else
		printf("Le parsing a retourné NULL (erreur détectée)\n");

	// Test 25: Espaces multiples dans la commande
	tokens = NULL;
	printf("\nTest 25: Espaces multiples dans la commande\n");
	printf("Commande: cat       file1    file2            >        file3\n");
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("file1", T_WORD, minishell));
	add_token(&tokens, create_token("file2", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("file3", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd_simple(result);
	else
		printf("Le parsing a retourné NULL (erreur détectée)\n");
}

// Fonction main pour exécuter les deux versions
int main(int argc, char **argv)
{
	t_minishell minishell;
	t_gc_head *gc;

	minishell.gc = (gc = gc_init());

	if (argc > 1 && ft_strcmp(argv[1], "simple") == 0)
	{
		// Version pour LLM
		test_parsing_simple(&minishell);
	}
	else
	{
		// Version avec couleurs
		test_parsing_colorized(&minishell);
	}
	gc_destroy(minishell.gc);
	return (0);
}
