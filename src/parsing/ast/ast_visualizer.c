/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_visualizer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 16:55:00 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/02 18:04:05 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/ast.h"
#include <stdio.h>

static const char *get_node_type_str(enum node_type type)
{
	switch (type)
	{
		case NODE_CMD:
			return ("CMD");
		case NODE_PIPE:
			return ("PIPE");
		case NODE_AND:
			return ("AND");
		case NODE_OR:
			return ("OR");
		case NODE_OPEN_PARENTHESIS:
			return ("OPEN_PARENTHESIS");
		case NODE_CLOSE_PARENTHESIS:
			return ("CLOSE_PARENTHESIS");
		default:
			return ("UNKNOWN");
	}
}

// Fonction récursive pour écrire les nœuds de l'AST dans le fichier DOT
void write_dot_file_recursive(FILE *dot_file, t_ast_node *node, int parent_id, int current_id);

void print_ast_compact(t_ast_node *node)
{
	if (!node)
		return;

	printf("(%s", get_node_type_str(node->type));
	if (node->type == NODE_CMD && node->cmd && node->cmd->command_raw)
		printf(":%s", node->cmd->command_raw);
	if (node->subshell)
		printf(":SUBSHELL");

	if (node->left || node->right)
	{
		printf(" ");
		if (node->left)
			print_ast_compact(node->left);
		else
			printf("NIL");

		printf(" ");
		if (node->right)
			print_ast_compact(node->right);
		else
			printf("NIL");
	}
	printf(")");
}

void print_ast_with_commands(t_ast_node *node, int depth)
{
	int i;

	if (!node)
		return;

	for (i = 0; i < depth; i++)
		printf("|  ");

	printf("+--");

	switch (node->type)
	{
		case NODE_CMD:
			printf("[CMD");
			if (node->cmd && node->cmd->command_raw)
			{
				printf(": %s", node->cmd->command_raw);
				if (node->cmd->redirs)
				{
					t_redir *redir = node->cmd->redirs;
					printf(" with redirections:");
					while (redir)
					{
						printf(" ");
						switch (redir->type)
						{
							case REDIR_IN:
								printf("<%s", redir->file_or_delimiter);
								break;
							case REDIR_OUT:
								printf(">%s", redir->file_or_delimiter);
								break;
							case REDIR_APPEND:
								printf(">>%s", redir->file_or_delimiter);
								break;
							case REDIR_HEREDOC:
								printf("<<%s", redir->file_or_delimiter);
								break;
						}
						redir = redir->next;
					}
				}
			}
			printf("]");
			break;
		case NODE_PIPE:
			printf("[PIPE]");
			break;
		case NODE_AND:
			printf("[AND]");
			break;
		case NODE_OR:
			printf("[OR]");
			break;
		case NODE_OPEN_PARENTHESIS:
			printf("[OPEN_PARENTHESIS]");
			break;
		case NODE_CLOSE_PARENTHESIS:
			printf("[CLOSE_PARENTHESIS]");
			break;
		default:
			printf("[UNKNOWN]");
	}

	if (node->subshell)
		printf(" (SUBSHELL)");

	printf("\n");

	print_ast_with_commands(node->left, depth + 1);
	print_ast_with_commands(node->right, depth + 1);
}

void print_ast_execution_order(t_ast_node *node)
{
	static int node_number = 1;

	if (!node)
		return;

	// Parcours infix pour l'ordre d'exécution
	if (node->left)
		print_ast_execution_order(node->left);

	printf("Node %d: ", node_number++);
	switch (node->type)
	{
		case NODE_CMD:
			printf("Execute command: %s",
				(node->cmd && node->cmd->command_raw) ? node->cmd->command_raw : "NULL");
			break;
		case NODE_PIPE:
			printf("Pipe output from left to right");
			break;
		case NODE_AND:
			printf("Logical AND - execute right only if left succeeds");
			break;
		case NODE_OR:
			printf("Logical OR - execute right only if left fails");
			break;
		default:
			printf("Special node type: %s", get_node_type_str(node->type));
	}

	if (node->subshell)
		printf(" (in subshell)");
	printf("\n");

	if (node->right)
		print_ast_execution_order(node->right);
}

void visualize_ast(t_ast_node *root, int mode)
{
	char    dot_filename[256];
	char    img_filename[256];

	if (!root)
	{
		printf("AST is empty (NULL)\n");
		return;
	}

	printf("\n=== AST Visualization ===\n");

	// Modes 0-2: visualisation textuelle
	if (mode >= 0 && mode <= 2)
	{
		switch (mode)
		{
			case 0: // Mode compact
				printf("Compact representation:\n");
				print_ast_compact(root);
				printf("\n");
				break;
			case 1: // Mode arborescent
				printf("Tree representation:\n");
				print_ast_with_commands(root, 0);
				break;
			case 2: // Mode ordre d'exécution
				printf("Execution order:\n");
				print_ast_execution_order(root);
				break;
			default:
				printf("Invalid visualization mode.\n");
		}
	}
	// Mode 3: génération d'image avec Graphviz
	else if (mode == 3)
	{
		printf("Generating GraphViz visualization...\n");

		// Créer les noms de fichiers
		snprintf(dot_filename, sizeof(dot_filename), "/tmp/ast_output.dot");
		snprintf(img_filename, sizeof(img_filename), "ast_output.png");

		// Générer le contenu du fichier DOT
		FILE *dot_file = fopen(dot_filename, "w");
		if (!dot_file)
		{
			printf("Error: Could not create DOT file\n");
			return;
		}

		// Écrire l'en-tête du fichier DOT
		fprintf(dot_file, "digraph AST {\n");
		fprintf(dot_file, "  node [shape=box, style=filled, fontname=\"Arial\"];\n");
		fprintf(dot_file, "  edge [fontname=\"Arial\"];\n\n");

		// Écrire les nœuds de l'AST récursivement
		write_dot_file_recursive(dot_file, root, 0, 0);

		// Fermer le graphe
		fprintf(dot_file, "}\n");
		fclose(dot_file);

		// Utiliser la commande dot pour générer l'image
		char cmd[512];
		sprintf(cmd, "dot -Tpng %s -o %s", dot_filename, img_filename);
		int result = system(cmd);

		if (result == 0)
			printf("Image generated successfully: %s\n", img_filename);
		else
			printf("Error generating image (is GraphViz installed?)\n");
	}
	else
	{
		printf("Invalid visualization mode.\n");
	}

	printf("===========================\n\n");
}

// Fonction récursive pour écrire les nœuds de l'AST dans le fichier DOT
void write_dot_file_recursive(FILE *dot_file, t_ast_node *node, int parent_id, int current_id)
{
	if (!node || !dot_file)
		return;

	// Déterminer la couleur du nœud en fonction de son type
	char *color;
	switch (node->type)
	{
		case NODE_CMD:
			color = "lightblue";
			break;
		case NODE_PIPE:
			color = "lightgreen";
			break;
		case NODE_AND:
			color = "orange";
			break;
		case NODE_OR:
			color = "yellow";
			break;
		default:
			color = "gray";
	}

	// Style spécial pour les sous-shells
	char style[100] = "filled";
	if (node->subshell)
		strcat(style, ",dashed");

	// Écrire le nœud actuel
	fprintf(dot_file, "  node%d [label=\"", current_id);

	// Label du nœud
	switch (node->type)
	{
		case NODE_CMD:
			if (node->cmd && node->cmd->command_raw)
				fprintf(dot_file, "%s", node->cmd->command_raw);
			else
				fprintf(dot_file, "CMD");
			break;
		case NODE_PIPE:
			fprintf(dot_file, "PIPE");
			break;
		case NODE_AND:
			fprintf(dot_file, "AND");
			break;
		case NODE_OR:
			fprintf(dot_file, "OR");
			break;
		default:
			fprintf(dot_file, "UNKNOWN");
	}

	// Indiquer les sous-shells
	if (node->subshell)
		fprintf(dot_file, "\\n(SUBSHELL)");

	fprintf(dot_file, "\", fillcolor=\"%s\", style=\"%s\"];\n", color, style);

	// Si ce n'est pas le nœud racine, relier au parent
	if (parent_id >= 0)
		fprintf(dot_file, "  node%d -> node%d;\n", parent_id, current_id);

	// ID pour les fils
	int left_id = current_id * 2 + 1;
	int right_id = current_id * 2 + 2;

	// Traiter les fils récursivement
	if (node->left)
		write_dot_file_recursive(dot_file, node->left, current_id, left_id);

	if (node->right)
		write_dot_file_recursive(dot_file, node->right, current_id, right_id);
}
