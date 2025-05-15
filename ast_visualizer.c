/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_visualizer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 16:55:00 by ufalzone          #+#    #+#             */
/*   Updated: 2025/05/14 18:29:50 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/ast.h"
#include <stdio.h>

static const char *get_node_type_str(enum e_node_type type)
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

	// Afficher les redirections associées au subshell
	if (node->subshell_redir)
	{
		for (i = 0; i <= depth; i++)
			printf("|  ");
		printf("   with subshell redirections:");
		t_redir *sr = node->subshell_redir;
		while (sr)
		{
			printf(" ");
			switch (sr->type)
			{
				case REDIR_IN: printf("<%s", sr->file_or_delimiter); break;
				case REDIR_OUT: printf(">%s", sr->file_or_delimiter); break;
				case REDIR_APPEND: printf(">>%s", sr->file_or_delimiter); break;
				case REDIR_HEREDOC: printf("<<%s", sr->file_or_delimiter); break;
			}
			sr = sr->next;
		}
		printf("\n");
	}

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
				printf("Ordre d'exécution:\n");
				print_ast_execution_order(root);
				break;
			default:
				printf("Invalid visualization mode.\n");
		}
	}
	// Mode 3: génération d'image avec Graphviz
	else if (mode == 3)
	{
		printf("Génération de la visualisation GraphViz...\n");

		// Créer les noms de fichiers
		snprintf(dot_filename, sizeof(dot_filename), "/tmp/ast_output.dot");
		snprintf(img_filename, sizeof(img_filename), "ast_output.png");

		// Générer le contenu du fichier DOT
		FILE *dot_file = fopen(dot_filename, "w");
		if (!dot_file)
		{
			printf("Erreur: Impossible de créer le fichier DOT\n");
			return;
		}

		// Écrire l'en-tête du fichier DOT avec un meilleur style pour la symétrie
		fprintf(dot_file, "digraph AST {\n");
		fprintf(dot_file, "  graph [\n");
		fprintf(dot_file, "    rankdir=TB,\n");
		fprintf(dot_file, "    fontname=\"Arial\",\n");
		fprintf(dot_file, "    bgcolor=\"#FAFAFA\",\n");
		fprintf(dot_file, "    nodesep=1.5,\n");
		fprintf(dot_file, "    ranksep=1.5,\n");
		fprintf(dot_file, "    splines=polyline,\n");
		fprintf(dot_file, "    ordering=out,\n");
		fprintf(dot_file, "    concentrate=true,\n");
		fprintf(dot_file, "    margin=\"0.5,0.5\",\n");
		fprintf(dot_file, "    pad=\"0.5,0.5\",\n");
		fprintf(dot_file, "    overlap=false\n");
		fprintf(dot_file, "  ];\n");
		fprintf(dot_file, "  node [\n");
		fprintf(dot_file, "    shape=box,\n");
		fprintf(dot_file, "    style=\"filled,rounded\",\n");
		fprintf(dot_file, "    fontname=\"Arial\",\n");
		fprintf(dot_file, "    fontsize=13,\n");
		fprintf(dot_file, "    margin=\"0.7,0.5\",\n");
		fprintf(dot_file, "    height=1.0,\n");
		fprintf(dot_file, "    width=2.0\n");
		fprintf(dot_file, "  ];\n");
		fprintf(dot_file, "  edge [\n");
		fprintf(dot_file, "    fontname=\"Arial\",\n");
		fprintf(dot_file, "    color=\"#444444\",\n");
		fprintf(dot_file, "    penwidth=1.5,\n");
		fprintf(dot_file, "    arrowsize=1.2,\n");
		fprintf(dot_file, "    minlen=2\n");
		fprintf(dot_file, "  ];\n\n");

		// Écrire les nœuds de l'AST récursivement
		write_dot_file_recursive(dot_file, root, 0, 0);

		// Fermer le graphe
		fprintf(dot_file, "}\n");
		fclose(dot_file);

		// Utiliser la commande dot pour générer l'image avec une meilleure qualité
		char cmd[512];
		sprintf(cmd, "dot -Tpng -Gdpi=500 -Gsize=16,12 -Gratio=auto %s -o %s", dot_filename, img_filename);
		int result = system(cmd);

		if (result == 0)
			printf("Image générée avec succès: %s\n", img_filename);
		else
			printf("Erreur lors de la génération de l'image (GraphViz est-il installé?)\n");
	}
	else
	{
		printf("Mode de visualisation invalide.\n");
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
			color = "#98FB98"; // Vert pâle
			break;
		case NODE_PIPE:
			color = "#87CEFA"; // Bleu clair
			break;
		case NODE_AND:
			color = "#FFA07A"; // Orange clair
			break;
		case NODE_OR:
			color = "#FFD700"; // Or
			break;
		default:
			color = "#D8BFD8"; // Violet pâle
	}

	// Appliquer une couleur plus foncée pour les sous-shells
	char final_color[32];
	strcpy(final_color, color);
	
	if (node->subshell)
	{
		// Pour les commandes en sous-shell, utiliser des couleurs plus foncées
		if (node->type == NODE_CMD)
			strcpy(final_color, "#67c967");  // Vert plus foncé
		else if (node->type == NODE_AND)
			strcpy(final_color, "#ff8c55");  // Orange plus foncé
		else if (node->type == NODE_OR)
			strcpy(final_color, "#ffbf00");  // Or plus foncé
		else if (node->type == NODE_PIPE)
			strcpy(final_color, "#5aaad8");  // Bleu plus foncé
	}

	// Style spécial pour les sous-shells avec bordure plus élégante
	char style[100] = "filled,rounded";
	if (node->subshell)
		strcat(style, ",dashed");

	// Traitement spécial pour les bordures des commandes avec redirections
	char border_attrs[256] = "";
	if (node->type == NODE_CMD && node->cmd && node->cmd->redirs)
	{
		t_redir *redir = node->cmd->redirs;
		char border_color[8] = "#3366ff"; // Bleu par défaut

		// Compter les différents types de redirections
		int in_count = 0, out_count = 0, append_count = 0, heredoc_count = 0;
		t_redir *temp_redir = redir;

		while (temp_redir) {
			switch (temp_redir->type) {
				case REDIR_IN: in_count++; break;
				case REDIR_OUT: out_count++; break;
				case REDIR_APPEND: append_count++; break;
				case REDIR_HEREDOC: heredoc_count++; break;
			}
			temp_redir = temp_redir->next;
		}

		// Déterminer la couleur de bordure en fonction des redirections présentes
		if (heredoc_count > 0)
			strcpy(border_color, "#9900cc"); // Violet pour heredoc (priorité la plus haute)
		else if (in_count > 0 && (out_count > 0 || append_count > 0))
			strcpy(border_color, "#ff6600"); // Orange pour entrée+sortie
		else if (in_count > 0)
			strcpy(border_color, "#009900"); // Vert foncé pour l'entrée uniquement
		else if (out_count > 0 || append_count > 0)
			strcpy(border_color, "#cc3300"); // Rouge pour la sortie uniquement

		// Définir l'épaisseur de la bordure en fonction du nombre total de redirections
		int total_redirs = in_count + out_count + append_count + heredoc_count;
		float border_width = 2.0 + (total_redirs > 3 ? 1.5 : total_redirs * 0.5);

		// Configurer les attributs de bordure
		sprintf(border_attrs, ", penwidth=%.1f, color=\"%s\"", border_width, border_color);
	}

	// Calculer la taille du nœud en fonction du type
	char size_attrs[64] = "";
	if (node->type == NODE_CMD)
	{
		// Commandes plus grandes pour mieux voir les détails
		sprintf(size_attrs, ", width=2.5, height=1.5, fixedsize=false");
	}
	else if (node->type == NODE_PIPE || node->type == NODE_AND || node->type == NODE_OR)
	{
		// Opérateurs plus adaptés
		sprintf(size_attrs, ", width=1.5, height=1.0, fixedsize=false");
	}

	// Écrire le nœud actuel
	fprintf(dot_file, "  node%d [label=<", current_id);

	// Label du nœud avec affichage des redirections en HTML pour un meilleur centrage
	switch (node->type)
	{
		case NODE_CMD:
			if (node->cmd)
			{
				fprintf(dot_file, "<TABLE BORDER=\"0\" CELLBORDER=\"0\" CELLSPACING=\"0\" CELLPADDING=\"2\">");

				// En-tête de la commande
				fprintf(dot_file, "<TR><TD ALIGN=\"center\"><FONT POINT-SIZE=\"15\" COLOR=\"#000080\"><B>COMMANDE</B></FONT></TD></TR>");
				fprintf(dot_file, "<TR><TD><BR/></TD></TR>");

				// Nom de la commande avec formatage
				if (node->cmd && node->cmd->command && node->cmd->command[0])
				{
					fprintf(dot_file, "<TR><TD ALIGN=\"center\">");
					fprintf(dot_file, "<FONT POINT-SIZE=\"14\" COLOR=\"#006400\"><B>≻ %s ≺</B></FONT>", node->cmd->command[0]);
					fprintf(dot_file, "</TD></TR>");

					// Ajouter les arguments supplémentaires
					int arg_count = 1;
					while (node->cmd->command[arg_count]) {
						if (arg_count == 1)
							fprintf(dot_file, "<TR><TD ALIGN=\"center\"><FONT POINT-SIZE=\"12\" COLOR=\"#303030\"><I>Arguments</I></FONT></TD></TR>");
						fprintf(dot_file, "<TR><TD ALIGN=\"left\"><FONT POINT-SIZE=\"11\" COLOR=\"#404040\">[%d]: %s</FONT></TD></TR>",
							arg_count, node->cmd->command[arg_count]);
						arg_count++;
					}
				}
				else
				{
					fprintf(dot_file, "<TR><TD ALIGN=\"center\"><FONT COLOR=\"#707070\">(Commande vide)</FONT></TD></TR>");
				}

				// Afficher les redirections
				if (node->cmd && node->cmd->redirs)
				{
					t_redir *redir = node->cmd->redirs;
					fprintf(dot_file, "<TR><TD><BR/></TD></TR>");
					fprintf(dot_file, "<TR><TD ALIGN=\"center\"><FONT POINT-SIZE=\"13\" COLOR=\"#800000\"><B>REDIRECTIONS</B></FONT></TD></TR>");
					int redir_count = 0;
					while (redir)
					{
						if (redir_count > 0)
							fprintf(dot_file, "<TR><TD><FONT POINT-SIZE=\"1\"> </FONT></TD></TR>");
						fprintf(dot_file, "<TR><TD ALIGN=\"left\"><FONT POINT-SIZE=\"12\"><B>• Type:</B> ");
						switch (redir->type)
						{
							case REDIR_IN:
								fprintf(dot_file, "<FONT COLOR=\"green\">ENTRÉE (&lt;)</FONT>");
								break;
							case REDIR_OUT:
								fprintf(dot_file, "<FONT COLOR=\"red\">SORTIE (&gt;)</FONT>");
								break;
							case REDIR_APPEND:
								fprintf(dot_file, "<FONT COLOR=\"orange\">APPEND (&gt;&gt;)</FONT>");
								break;
							case REDIR_HEREDOC:
								fprintf(dot_file, "<FONT COLOR=\"purple\">HEREDOC (&lt;&lt;)</FONT>");
								break;
							default:
								fprintf(dot_file, "<FONT COLOR=\"gray\">INCONNU (?)</FONT>");
						}
						fprintf(dot_file, "</FONT></TD></TR>");
						fprintf(dot_file, "<TR><TD ALIGN=\"left\"><FONT POINT-SIZE=\"12\"><B>• Fichier:</B> %s</FONT></TD></TR>",
							redir->file_or_delimiter ? redir->file_or_delimiter : "(aucun)");
						redir = redir->next;
						redir_count++;
					}
				}

				// Indication de subshell si nécessaire
				if (node->subshell)
				{
					fprintf(dot_file, "<TR><TD><BR/></TD></TR>");
					fprintf(dot_file, "<TR><TD ALIGN=\"center\"><FONT POINT-SIZE=\"13\" COLOR=\"#444444\"><B>⟦ SUBSHELL ⟧</B></FONT></TD></TR>");
				}

				// Affichage des redirections de subshell
				if (node->subshell_redir)
				{
					if (!node->subshell)
						fprintf(dot_file, "<TR><TD><BR/></TD></TR>");
						
					fprintf(dot_file, "<TR><TD ALIGN=\"center\"><FONT POINT-SIZE=\"13\" COLOR=\"#800080\"><B>REDIR SUBSHELL</B></FONT></TD></TR>");
					t_redir *redir = node->subshell_redir;
					int redir_count = 0;
					while (redir)
					{
						if (redir_count > 0)
							fprintf(dot_file, "<TR><TD><FONT POINT-SIZE=\"1\"> </FONT></TD></TR>");
						fprintf(dot_file, "<TR><TD ALIGN=\"left\"><FONT POINT-SIZE=\"12\"><B>• Type:</B> ");
						switch (redir->type)
						{
							case REDIR_IN:
								fprintf(dot_file, "<FONT COLOR=\"green\">ENTRÉE (&lt;)</FONT>");
								break;
							case REDIR_OUT:
								fprintf(dot_file, "<FONT COLOR=\"red\">SORTIE (&gt;)</FONT>");
								break;
							case REDIR_APPEND:
								fprintf(dot_file, "<FONT COLOR=\"orange\">APPEND (&gt;&gt;)</FONT>");
								break;
							case REDIR_HEREDOC:
								fprintf(dot_file, "<FONT COLOR=\"purple\">HEREDOC (&lt;&lt;)</FONT>");
								break;
							default:
								fprintf(dot_file, "<FONT COLOR=\"gray\">INCONNU (?)</FONT>");
						}
						fprintf(dot_file, "</FONT></TD></TR>");
						fprintf(dot_file, "<TR><TD ALIGN=\"left\"><FONT POINT-SIZE=\"12\"><B>• Fichier:</B> %s</FONT></TD></TR>",
							redir->file_or_delimiter ? redir->file_or_delimiter : "(aucun)");
						redir = redir->next;
						redir_count++;
					}
				}

				fprintf(dot_file, "</TABLE>");
			}
			else
			{
				fprintf(dot_file, "<TABLE BORDER=\"0\" CELLBORDER=\"0\" CELLSPACING=\"0\" CELLPADDING=\"2\">");
				fprintf(dot_file, "<TR><TD ALIGN=\"center\"><FONT POINT-SIZE=\"15\" COLOR=\"#000080\"><B>COMMANDE</B></FONT></TD></TR>");
				fprintf(dot_file, "<TR><TD><BR/></TD></TR>");
				fprintf(dot_file, "<TR><TD ALIGN=\"center\"><FONT COLOR=\"#707070\">(Aucune commande)</FONT></TD></TR>");
				fprintf(dot_file, "</TABLE>");
			}
			break;
			
		case NODE_PIPE:
			fprintf(dot_file, "<TABLE BORDER=\"0\" CELLBORDER=\"0\" CELLSPACING=\"0\" CELLPADDING=\"2\">");
			fprintf(dot_file, "<TR><TD ALIGN=\"center\"><FONT POINT-SIZE=\"18\" COLOR=\"#000080\"><B>|</B></FONT></TD></TR>");
			fprintf(dot_file, "<TR><TD ALIGN=\"center\"><FONT POINT-SIZE=\"14\" COLOR=\"#000080\"><B>PIPE</B></FONT></TD></TR>");
			
			if (node->subshell)
			{
				fprintf(dot_file, "<TR><TD><BR/></TD></TR>");
				fprintf(dot_file, "<TR><TD ALIGN=\"center\"><FONT POINT-SIZE=\"13\" COLOR=\"#444444\"><B>⟦ SUBSHELL ⟧</B></FONT></TD></TR>");
			}
			
			// Affichage des redirections de subshell
			if (node->subshell_redir)
			{
				if (!node->subshell)
					fprintf(dot_file, "<TR><TD><BR/></TD></TR>");
					
				fprintf(dot_file, "<TR><TD ALIGN=\"center\"><FONT POINT-SIZE=\"13\" COLOR=\"#800080\"><B>REDIR SUBSHELL</B></FONT></TD></TR>");
				t_redir *redir = node->subshell_redir;
				int redir_count = 0;
				while (redir)
				{
					if (redir_count > 0)
						fprintf(dot_file, "<TR><TD><FONT POINT-SIZE=\"1\"> </FONT></TD></TR>");
					fprintf(dot_file, "<TR><TD ALIGN=\"left\"><FONT POINT-SIZE=\"12\"><B>• Type:</B> ");
					switch (redir->type)
					{
						case REDIR_IN:
							fprintf(dot_file, "<FONT COLOR=\"green\">ENTRÉE (&lt;)</FONT>");
							break;
						case REDIR_OUT:
							fprintf(dot_file, "<FONT COLOR=\"red\">SORTIE (&gt;)</FONT>");
							break;
						case REDIR_APPEND:
							fprintf(dot_file, "<FONT COLOR=\"orange\">APPEND (&gt;&gt;)</FONT>");
							break;
						case REDIR_HEREDOC:
							fprintf(dot_file, "<FONT COLOR=\"purple\">HEREDOC (&lt;&lt;)</FONT>");
							break;
						default:
							fprintf(dot_file, "<FONT COLOR=\"gray\">INCONNU (?)</FONT>");
					}
					fprintf(dot_file, "</FONT></TD></TR>");
					fprintf(dot_file, "<TR><TD ALIGN=\"left\"><FONT POINT-SIZE=\"12\"><B>• Fichier:</B> %s</FONT></TD></TR>",
						redir->file_or_delimiter ? redir->file_or_delimiter : "(aucun)");
					redir = redir->next;
					redir_count++;
				}
			}
			
			fprintf(dot_file, "</TABLE>");
			break;
			
		case NODE_AND:
			fprintf(dot_file, "<TABLE BORDER=\"0\" CELLBORDER=\"0\" CELLSPACING=\"0\" CELLPADDING=\"2\">");
			fprintf(dot_file, "<TR><TD ALIGN=\"center\"><FONT POINT-SIZE=\"18\" COLOR=\"#000080\"><B>&amp;&amp;</B></FONT></TD></TR>");
			fprintf(dot_file, "<TR><TD ALIGN=\"center\"><FONT POINT-SIZE=\"14\" COLOR=\"#000080\"><B>AND</B></FONT></TD></TR>");
			
			if (node->subshell)
			{
				fprintf(dot_file, "<TR><TD><BR/></TD></TR>");
				fprintf(dot_file, "<TR><TD ALIGN=\"center\"><FONT POINT-SIZE=\"13\" COLOR=\"#444444\"><B>⟦ SUBSHELL ⟧</B></FONT></TD></TR>");
			}
			
			if (node->subshell_redir)
			{
				if (!node->subshell)
					fprintf(dot_file, "<TR><TD><BR/></TD></TR>");
					
				fprintf(dot_file, "<TR><TD ALIGN=\"center\"><FONT POINT-SIZE=\"13\" COLOR=\"#800080\"><B>REDIR SUBSHELL</B></FONT></TD></TR>");
				t_redir *redir = node->subshell_redir;
				int redir_count = 0;
				while (redir)
				{
					if (redir_count > 0)
						fprintf(dot_file, "<TR><TD><FONT POINT-SIZE=\"1\"> </FONT></TD></TR>");
					fprintf(dot_file, "<TR><TD ALIGN=\"left\"><FONT POINT-SIZE=\"12\"><B>• Type:</B> ");
					switch (redir->type)
					{
						case REDIR_IN:
							fprintf(dot_file, "<FONT COLOR=\"green\">ENTRÉE (&lt;)</FONT>");
							break;
						case REDIR_OUT:
							fprintf(dot_file, "<FONT COLOR=\"red\">SORTIE (&gt;)</FONT>");
							break;
						case REDIR_APPEND:
							fprintf(dot_file, "<FONT COLOR=\"orange\">APPEND (&gt;&gt;)</FONT>");
							break;
						case REDIR_HEREDOC:
							fprintf(dot_file, "<FONT COLOR=\"purple\">HEREDOC (&lt;&lt;)</FONT>");
							break;
						default:
							fprintf(dot_file, "<FONT COLOR=\"gray\">INCONNU (?)</FONT>");
					}
					fprintf(dot_file, "</FONT></TD></TR>");
					fprintf(dot_file, "<TR><TD ALIGN=\"left\"><FONT POINT-SIZE=\"12\"><B>• Fichier:</B> %s</FONT></TD></TR>",
						redir->file_or_delimiter ? redir->file_or_delimiter : "(aucun)");
					redir = redir->next;
					redir_count++;
				}
			}
			
			fprintf(dot_file, "</TABLE>");
			break;
			
		case NODE_OR:
			fprintf(dot_file, "<TABLE BORDER=\"0\" CELLBORDER=\"0\" CELLSPACING=\"0\" CELLPADDING=\"2\">");
			fprintf(dot_file, "<TR><TD ALIGN=\"center\"><FONT POINT-SIZE=\"18\" COLOR=\"#000080\"><B>||</B></FONT></TD></TR>");
			fprintf(dot_file, "<TR><TD ALIGN=\"center\"><FONT POINT-SIZE=\"14\" COLOR=\"#000080\"><B>OR</B></FONT></TD></TR>");
			
			if (node->subshell)
			{
				fprintf(dot_file, "<TR><TD><BR/></TD></TR>");
				fprintf(dot_file, "<TR><TD ALIGN=\"center\"><FONT POINT-SIZE=\"13\" COLOR=\"#444444\"><B>⟦ SUBSHELL ⟧</B></FONT></TD></TR>");
			}
			
			if (node->subshell_redir)
			{
				if (!node->subshell)
					fprintf(dot_file, "<TR><TD><BR/></TD></TR>");
					
				fprintf(dot_file, "<TR><TD ALIGN=\"center\"><FONT POINT-SIZE=\"13\" COLOR=\"#800080\"><B>REDIR SUBSHELL</B></FONT></TD></TR>");
				t_redir *redir = node->subshell_redir;
				int redir_count = 0;
				while (redir)
				{
					if (redir_count > 0)
						fprintf(dot_file, "<TR><TD><FONT POINT-SIZE=\"1\"> </FONT></TD></TR>");
					fprintf(dot_file, "<TR><TD ALIGN=\"left\"><FONT POINT-SIZE=\"12\"><B>• Type:</B> ");
					switch (redir->type)
					{
						case REDIR_IN:
							fprintf(dot_file, "<FONT COLOR=\"green\">ENTRÉE (&lt;)</FONT>");
							break;
						case REDIR_OUT:
							fprintf(dot_file, "<FONT COLOR=\"red\">SORTIE (&gt;)</FONT>");
							break;
						case REDIR_APPEND:
							fprintf(dot_file, "<FONT COLOR=\"orange\">APPEND (&gt;&gt;)</FONT>");
							break;
						case REDIR_HEREDOC:
							fprintf(dot_file, "<FONT COLOR=\"purple\">HEREDOC (&lt;&lt;)</FONT>");
							break;
						default:
							fprintf(dot_file, "<FONT COLOR=\"gray\">INCONNU (?)</FONT>");
					}
					fprintf(dot_file, "</FONT></TD></TR>");
					fprintf(dot_file, "<TR><TD ALIGN=\"left\"><FONT POINT-SIZE=\"12\"><B>• Fichier:</B> %s</FONT></TD></TR>",
						redir->file_or_delimiter ? redir->file_or_delimiter : "(aucun)");
					redir = redir->next;
					redir_count++;
				}
			}
			
			fprintf(dot_file, "</TABLE>");
			break;
			
		case NODE_OPEN_PARENTHESIS:
			fprintf(dot_file, "<TABLE BORDER=\"0\" CELLBORDER=\"0\" CELLSPACING=\"0\" CELLPADDING=\"2\">");
			fprintf(dot_file, "<TR><TD ALIGN=\"center\"><FONT POINT-SIZE=\"25\" COLOR=\"#000080\"><B>(</B></FONT></TD></TR>");
			fprintf(dot_file, "<TR><TD ALIGN=\"center\"><FONT POINT-SIZE=\"14\" COLOR=\"#000080\"><B>OUVERTURE</B></FONT></TD></TR>");
			fprintf(dot_file, "</TABLE>");
			break;
			
		case NODE_CLOSE_PARENTHESIS:
			fprintf(dot_file, "<TABLE BORDER=\"0\" CELLBORDER=\"0\" CELLSPACING=\"0\" CELLPADDING=\"2\">");
			fprintf(dot_file, "<TR><TD ALIGN=\"center\"><FONT POINT-SIZE=\"25\" COLOR=\"#000080\"><B>)</B></FONT></TD></TR>");
			fprintf(dot_file, "<TR><TD ALIGN=\"center\"><FONT POINT-SIZE=\"14\" COLOR=\"#000080\"><B>FERMETURE</B></FONT></TD></TR>");
			fprintf(dot_file, "</TABLE>");
			break;
			
		default:
			fprintf(dot_file, "<TABLE BORDER=\"0\" CELLBORDER=\"0\" CELLSPACING=\"0\" CELLPADDING=\"2\">");
			fprintf(dot_file, "<TR><TD ALIGN=\"center\"><FONT POINT-SIZE=\"15\" COLOR=\"#880000\"><B>INCONNU</B></FONT></TD></TR>");
			fprintf(dot_file, "<TR><TD ALIGN=\"center\"><FONT POINT-SIZE=\"12\">(%d)</FONT></TD></TR>", node->type);
			fprintf(dot_file, "</TABLE>");
	}

	// Fermer le label et définir le style
	fprintf(dot_file, ">, fillcolor=\"%s\", style=\"%s\"%s%s];\n",
		final_color, style, border_attrs, size_attrs);

	// Si ce n'est pas le nœud racine, relier au parent avec des contraintes pour améliorer la symétrie
	if (parent_id >= 0)
	{
		// Définir les styles de flèche en fonction du type de relation
		char *edge_style = "solid";
		char *edge_color = "#444444";
		float edge_width = 1.5;
		
		// Personnaliser les flèches en fonction du type de nœud parent
		if (node->parent != NULL)
		{
			if (node == node->parent->left) // Fils gauche
			{
				if (node->parent->type == NODE_PIPE)
				{
					edge_color = "#0066CC"; // Bleu pour les pipes
					edge_width = 2.0;
				}
				else if (node->parent->type == NODE_AND)
				{
					edge_color = "#009900"; // Vert pour AND
				}
				else if (node->parent->type == NODE_OR)
				{
					edge_color = "#CC6600"; // Orange pour OR
				}
			}
			else if (node == node->parent->right) // Fils droit
			{
				if (node->parent->type == NODE_PIPE)
				{
					edge_color = "#0066CC"; // Bleu pour les pipes
					edge_style = "dashed";
					edge_width = 2.0;
				}
				else if (node->parent->type == NODE_AND)
				{
					edge_color = "#009900"; // Vert pour AND
					edge_style = "dashed";
				}
				else if (node->parent->type == NODE_OR)
				{
					edge_color = "#CC6600"; // Orange pour OR
					edge_style = "dashed";
				}
			}
		}
		
		fprintf(dot_file, "  node%d -> node%d [color=\"%s\", style=\"%s\", penwidth=%.1f, ",
			parent_id, current_id, edge_color, edge_style, edge_width);

		// Ajuster les contraintes de placement pour une meilleure symétrie
		if (current_id == parent_id * 2 + 1) // Si c'est un fils gauche
			fprintf(dot_file, "weight=10, constraint=true, headlabel=\"G\", labelfontsize=10, labelangle=-25, labeldistance=2.0");
		else
			fprintf(dot_file, "weight=10, constraint=true, headlabel=\"D\", labelfontsize=10, labelangle=25, labeldistance=2.0");

		fprintf(dot_file, "];\n");
	}

	// ID pour les fils
	int left_id = current_id * 2 + 1;
	int right_id = current_id * 2 + 2;

	// Traiter les fils récursivement
	if (node->left)
	{
		// Stocker une référence au parent dans le nœud enfant pour faciliter le stylage des arêtes
		node->left->parent = node;
		write_dot_file_recursive(dot_file, node->left, current_id, left_id);
	}

	if (node->right)
	{
		// Stocker une référence au parent dans le nœud enfant
		node->right->parent = node;
		write_dot_file_recursive(dot_file, node->right, current_id, right_id);
	}
}
