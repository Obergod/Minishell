/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parsing_versions.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:00:00 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/20 16:28:53 by ufalzone         ###   ########.fr       */
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
		print_cmd(result, 10);
	else
		printf("%sLe parsing a retourné NULL (erreur détectée)%s\n", red, reset);

	// Test 11: Pipeline complexe avec redirections multiples
	tokens = NULL;
	printf("\n%s=== Test 11: Pipeline complexe avec redirections multiples ===\n", cyan);
	printf("Commande: cat < in1.txt < in2.txt | grep pattern > out1.txt | sort | uniq >> out2.txt << EOF | wc -l%s\n", reset);
	printf("%s(Ce test vérifie la gestion des redirections multiples dans un pipeline complexe)%s\n", yellow, reset);
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("in1.txt", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("in2.txt", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("grep", T_WORD, minishell));
	add_token(&tokens, create_token("pattern", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("out1.txt", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("sort", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("uniq", T_WORD, minishell));
	add_token(&tokens, create_token(">>", T_REDIR, minishell));
	add_token(&tokens, create_token("out2.txt", T_WORD, minishell));
	add_token(&tokens, create_token("<<", T_REDIR, minishell));
	add_token(&tokens, create_token("EOF", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("wc", T_WORD, minishell));
	add_token(&tokens, create_token("-l", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
	{
		t_cmd *current = result;
		int cmd_num = 1;
		while (current)
		{
			printf("%sCommande %d:%s\n", cyan, cmd_num++, reset);
			print_cmd(current, 11);
			current = current->next;
		}
	}
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
	char *red = "\033[0;31m";     // Rouge pour les erreurs
	char *reset = "\033[0m";     // Réinitialisation de la couleur

	printf("=== TESTS DE PARSING COMPLEXES ===\n");

	// Test 1: Opérateurs logiques consécutifs
	printf("\nTest 1: Opérateurs logiques consécutifs\n");
	printf("Commande: ls && && grep test\n");
	printf("Résultat attendu: ERREUR (ERR_SYNTAX_LOGIC)\n");
	add_token(&tokens, create_token("ls", T_WORD, minishell));
	add_token(&tokens, create_token("&&", T_LOGIC, minishell));
	add_token(&tokens, create_token("&&", T_LOGIC, minishell));
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

	// Test 2: Chaînes vides et espaces
	tokens = NULL;
	printf("\nTest 2: Chaînes vides et espaces\n");
	printf("Commande: echo \"\" '' \" \" \"  \"\n");
	printf("Résultat attendu: Commande avec 5 arguments vides\n");
	add_token(&tokens, create_token("echo", T_WORD, minishell));
	add_token(&tokens, create_token("", T_WORD, minishell));
	add_token(&tokens, create_token("", T_WORD, minishell));
	add_token(&tokens, create_token(" ", T_WORD, minishell));
	add_token(&tokens, create_token("  ", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd_simple(result);
	else
		printf("Le parsing a retourné NULL (erreur détectée)\n");

	// Test 3: Redirections multiples et complexes
	tokens = NULL;
	printf("\nTest 3: Redirections multiples et complexes\n");
	printf("Commande: cat < file1.txt < file2.txt > out1.txt > out2.txt >> append.txt%s\n", reset);
	printf("Résultat attendu: Dernière redirection de chaque type prise en compte\n");
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("file1.txt", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("file2.txt", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("out1.txt", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("out2.txt", T_WORD, minishell));
	add_token(&tokens, create_token(">>", T_REDIR, minishell));
	add_token(&tokens, create_token("append.txt", T_WORD, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd(result, 3);
	else
		printf("%sLe parsing a retourné NULL (erreur détectée)%s\n", red, reset);

	// Test 4: Pipes consécutifs
	tokens = NULL;
	printf("\nTest 4: Pipes consécutifs\n");
	printf("Commande: ls | | grep test\n");
	printf("Résultat attendu: ERREUR (ERR_SYNTAX_PIPE)\n");
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

	// Test 5: Opérateurs logiques au début/fin
	tokens = NULL;
	printf("\nTest 5: Opérateurs logiques au début/fin\n");
	printf("Commande: && ls || grep test ||\n");
	printf("Résultat attendu: ERREUR (ERR_SYNTAX_LOGIC)\n");
	add_token(&tokens, create_token("&&", T_LOGIC, minishell));
	add_token(&tokens, create_token("ls", T_WORD, minishell));
	add_token(&tokens, create_token("||", T_LOGIC, minishell));
	add_token(&tokens, create_token("grep", T_WORD, minishell));
	add_token(&tokens, create_token("test", T_WORD, minishell));
	add_token(&tokens, create_token("||", T_LOGIC, minishell));

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

	// Test 6: Heredoc avec pipe et redirection
	tokens = NULL;
	printf("\nTest 6: Heredoc avec pipe et redirection\n");
	printf("Commande: cat <<EOF | wc -l > out.txt\n");
	printf("Résultat attendu: Commande avec heredoc, pipe et redirection\n");
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

	// Test 7: Guillemets échappés et noms de fichiers avec espaces
	tokens = NULL;
	printf("\nTest 7: Guillemets échappés et noms de fichiers avec espaces\n");
	printf("Commande: grep \\\"hello world\\\" < \\\"input file.txt\\\" >> \\\"my output file.txt\\\"%s\n", reset);
	printf("Résultat attendu: Commande avec arguments échappés et fichiers avec espaces\n");
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

	// Test 8: Pipeline complexe avec opérateurs logiques
	tokens = NULL;
	printf("\nTest 8: Pipeline complexe avec opérateurs logiques\n");
	printf("Commande: ls -la | grep test && cat file.txt || wc -l\n");
	printf("Résultat attendu: Pipeline avec opérateurs logiques\n");
	add_token(&tokens, create_token("ls", T_WORD, minishell));
	add_token(&tokens, create_token("-la", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("grep", T_WORD, minishell));
	add_token(&tokens, create_token("test", T_WORD, minishell));
	add_token(&tokens, create_token("&&", T_LOGIC, minishell));
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("file.txt", T_WORD, minishell));
	add_token(&tokens, create_token("||", T_LOGIC, minishell));
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

	// Test 9: Redirections sans fichier
	tokens = NULL;
	printf("\nTest 9: Redirections sans fichier\n");
	printf("Commande: cat < > >>\n");
	printf("Résultat attendu: ERREUR (ERR_SYNTAX_REDIRECT)\n");
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token(">>", T_REDIR, minishell));

	result = parsing(tokens, minishell);
	if (result)
		print_cmd_simple(result);
	else
		printf("Le parsing a retourné NULL (erreur détectée)\n");

	// Test 10: Commandes vides avec opérateurs
	tokens = NULL;
	printf("\nTest 10: Commandes vides avec opérateurs\n");
	printf("Commande: \"\" && \"\" || \"\"\n");
	printf("Résultat attendu: Commande avec arguments vides\n");
	add_token(&tokens, create_token("", T_WORD, minishell));
	add_token(&tokens, create_token("&&", T_LOGIC, minishell));
	add_token(&tokens, create_token("", T_WORD, minishell));
	add_token(&tokens, create_token("||", T_LOGIC, minishell));
	add_token(&tokens, create_token("", T_WORD, minishell));

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

	// Test 11: Heredoc avec pipe et redirection multiple
	tokens = NULL;
	printf("\nTest 11: Heredoc avec pipe et redirection multiple\n");
	printf("Commande: cat <<EOF | grep test > out1.txt >> out2.txt\n");
	printf("Résultat attendu: Commande avec heredoc, pipe et redirections multiples\n");
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("<<", T_REDIR, minishell));
	add_token(&tokens, create_token("EOF", T_WORD, minishell));
	add_token(&tokens, create_token("|", T_PIPE, minishell));
	add_token(&tokens, create_token("grep", T_WORD, minishell));
	add_token(&tokens, create_token("test", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("out1.txt", T_WORD, minishell));
	add_token(&tokens, create_token(">>", T_REDIR, minishell));
	add_token(&tokens, create_token("out2.txt", T_WORD, minishell));

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

	// Test 12: Opérateurs logiques avec redirections
	tokens = NULL;
	printf("\nTest 12: Opérateurs logiques avec redirections\n");
	printf("Commande: ls > out.txt && cat < in.txt || grep test >> log.txt\n");
	printf("Résultat attendu: Commandes avec opérateurs logiques et redirections\n");
	add_token(&tokens, create_token("ls", T_WORD, minishell));
	add_token(&tokens, create_token(">", T_REDIR, minishell));
	add_token(&tokens, create_token("out.txt", T_WORD, minishell));
	add_token(&tokens, create_token("&&", T_LOGIC, minishell));
	add_token(&tokens, create_token("cat", T_WORD, minishell));
	add_token(&tokens, create_token("<", T_REDIR, minishell));
	add_token(&tokens, create_token("in.txt", T_WORD, minishell));
	add_token(&tokens, create_token("||", T_LOGIC, minishell));
	add_token(&tokens, create_token("grep", T_WORD, minishell));
	add_token(&tokens, create_token("test", T_WORD, minishell));
	add_token(&tokens, create_token(">>", T_REDIR, minishell));
	add_token(&tokens, create_token("log.txt", T_WORD, minishell));

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
