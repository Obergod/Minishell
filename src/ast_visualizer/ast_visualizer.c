/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_visualizer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 00:00:00 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/02 22:50:23 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast_visualizer.h"
#include <string.h>

/**
 * Fonction principale pour visualiser un AST
 *
 * @param ast Pointeur vers l'arbre à visualiser
 * @param mode Mode de visualisation: 0=compact, 1=arborescent, 2=ordre d'exécution, 3=graphviz
 */
void    visualize_ast(t_ast_node *ast, int mode)
{
    char    dot_filename[256];
    char    img_filename[256];

    if (!ast)
    {
        printf("AST is empty (NULL)\n");
        return;
    }

    // Mode 0-2: utiliser les fonctions de visualisation textuelles
    if (mode >= 0 && mode <= 2)
    {
        printf("\n=== AST Visualization ===\n");

        switch (mode)
        {
            case 0: // Mode compact
                printf("Compact representation:\n");
                print_ast_compact(ast);
                printf("\n");
                break;
            case 1: // Mode arborescent
                printf("Tree representation:\n");
                print_ast_with_commands(ast, 0);
                break;
            case 2: // Mode ordre d'exécution
                printf("Execution order:\n");
                print_ast_execution_order(ast);
                break;
            default:
                printf("Invalid visualization mode.\n");
        }

        printf("===========================\n\n");
        return;
    }

    // Mode 3: génération d'image avec Graphviz
    const char *output_filename = "ast_output";

    // Créer les noms de fichiers
    snprintf(dot_filename, sizeof(dot_filename), "/tmp/%s.dot", output_filename);
    snprintf(img_filename, sizeof(img_filename), "%s.png", output_filename);

    // Génère le fichier DOT
    generate_dot_file(ast, dot_filename);

    // Génère l'image
    render_ast_image(dot_filename, img_filename);
}

/**
 * Génère un fichier DOT représentant l'AST
 *
 * @param ast Pointeur vers l'arbre à visualiser
 * @param filename Chemin du fichier DOT à générer
 */
void    generate_dot_file(t_ast_node *ast, const char *filename)
{
    FILE    *dot_file;
    int     node_counter = 0;

    if (!ast || !filename)
        return;

    dot_file = fopen(filename, "w");
    if (!dot_file)
    {
        perror("Error opening dot file");
        return;
    }

    // Écrire l'en-tête du fichier DOT
    write_dot_header(dot_file);

    // Écrire récursivement les nœuds de l'AST
    write_ast_recursive(dot_file, ast, -1, 0, &node_counter);

    // Écrire le pied de page du fichier DOT
    write_dot_footer(dot_file);

    fclose(dot_file);
}

/**
 * Appelle Graphviz pour générer une image à partir du fichier DOT
 *
 * @param dot_filename Chemin vers le fichier DOT
 * @param output_filename Chemin vers le fichier image à générer
 */
void    render_ast_image(const char *dot_filename, const char *output_filename)
{
    char    command[512];

    if (!dot_filename || !output_filename)
        return;

    // Construire la commande pour générer l'image avec une meilleure qualité
    snprintf(command, sizeof(command),
             "dot -Tpng -Gdpi=400 -Gsize=10,10 -Gratio=fill -o%s %s && echo \"AST visualisé dans %s\"",
             output_filename, dot_filename, output_filename);

    // Exécuter la commande
    if (system(command) != 0)
        perror("Error generating AST image");
}

/**
 * Écrit l'en-tête du fichier DOT
 *
 * @param dot_file Pointeur vers le fichier DOT ouvert
 */
void    write_dot_header(FILE *dot_file)
{
    fprintf(dot_file, "digraph AST {\n");
    fprintf(dot_file, "  graph [rankdir=TB, fontname=\"Arial\", bgcolor=\"#F8F8F8\", nodesep=1.0, ranksep=1.0, splines=ortho];\n");
    fprintf(dot_file, "  node [shape=box, style=\"filled,rounded\", fontname=\"Arial\", fontsize=12, margin=\"0.4,0.3\"];\n");
    fprintf(dot_file, "  edge [fontname=\"Arial\", color=\"#555555\", penwidth=1.5, arrowsize=1.2];\n\n");
}

/**
 * Écrit le pied de page du fichier DOT
 *
 * @param dot_file Pointeur vers le fichier DOT ouvert
 */
void    write_dot_footer(FILE *dot_file)
{
    fprintf(dot_file, "}\n");
}

/**
 * Écrit récursivement les nœuds de l'AST dans le fichier DOT
 *
 * @param dot_file Pointeur vers le fichier DOT ouvert
 * @param node Nœud actuel à écrire
 * @param parent_id ID du nœud parent (-1 pour la racine)
 * @param current_id ID du nœud actuel
 * @param node_counter Compteur pour générer des IDs uniques
 */
void    write_ast_recursive(FILE *dot_file, t_ast_node *node,
                       int parent_id, int current_id, int *node_counter)
{
    if (!node || !dot_file || !node_counter)
        return;

    // Écrire le nœud actuel
    write_node(dot_file, node, node_counter);

    // Connexion au parent
    if (parent_id >= 0)
    {
        fprintf(dot_file, "  node%d -> node%d;\n",
                parent_id, current_id);
    }

    // Fils gauche
    if (node->left)
    {
        (*node_counter)++;
        write_ast_recursive(dot_file, node->left, current_id, *node_counter, node_counter);
    }

    // Fils droit
    if (node->right)
    {
        (*node_counter)++;
        write_ast_recursive(dot_file, node->right, current_id, *node_counter, node_counter);
    }
}

/**
 * Écrit un nœud dans le fichier DOT
 *
 * @param dot_file Pointeur vers le fichier DOT ouvert
 * @param node Nœud à écrire
 * @param node_counter Compteur pour générer des IDs uniques
 */
void    write_node(FILE *dot_file, t_ast_node *node, int *node_counter)
{
    if (!node || !dot_file || !node_counter)
        return;

    // Vérifier que le type de nœud est valide
    if (node->type < NODE_NONE || node->type > NODE_CLOSE_PARENTHESIS)
    {
        fprintf(dot_file, "  node%d [label=\"INVALID TYPE %d\", fillcolor=\"#FF0000\"];\n",
            *node_counter, node->type);
        return;
    }

    char *label = get_node_label(node);
    char *color = get_node_color(node->type);

    // Configuration avancée du style en fonction du type de nœud
    char node_style[256] = "filled"; // Style de base
    char extra_attrs[512] = "";      // Attributs supplémentaires

    // Couleur de base
    char final_color[32];
    strcpy(final_color, color);

    // Traitement spécial pour les sous-shells
    if (node->subshell)
    {
        // Pour les commandes en sous-shell, utiliser un vert plus foncé
        if (node->type == NODE_CMD)
            strcpy(final_color, "#67c967");  // Vert plus foncé
        // Pour les opérateurs en sous-shell, utiliser des couleurs plus foncées
        else if (node->type == NODE_AND)
            strcpy(final_color, "#ff8c55");  // Orange plus foncé
        else if (node->type == NODE_OR)
            strcpy(final_color, "#ffbf00");  // Or plus foncé
        else if (node->type == NODE_PIPE)
            strcpy(final_color, "#5aaad8");  // Bleu plus foncé

        // Ajouter une bordure en pointillés pour les sous-shells
        strcat(node_style, ",rounded");
        sprintf(extra_attrs, ", penwidth=2, style=\"filled,rounded,dashed\", color=\"#444444\"");
    }
    else
    {
        strcat(node_style, node->type == NODE_CMD ? ",rounded" : "");
    }

    // Traitement spécial pour les commandes avec redirections
    if (node->type == NODE_CMD && node->cmd && node->cmd->redirs)
    {
        t_redir *redir = node->cmd->redirs;
        char border_color[8];

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
        else
            strcpy(border_color, "#3366ff"); // Bleu par défaut (ne devrait pas arriver)

        // Définir l'épaisseur de la bordure en fonction du nombre total de redirections
        int total_redirs = in_count + out_count + append_count + heredoc_count;
        float border_width = 2.0 + (total_redirs > 3 ? 1.5 : total_redirs * 0.5);

        // Si le nœud a déjà des attributs pour sous-shell, ajouter à ceux-ci
        if (node->subshell)
            sprintf(extra_attrs, ", penwidth=%.1f, style=\"filled,rounded,dashed\", color=\"%s\"",
                    border_width, border_color);
        else
            sprintf(extra_attrs, ", penwidth=%.1f, style=\"filled,rounded\", color=\"%s\"",
                    border_width, border_color);
    }

    // Écrire le nœud avec tous ses attributs
    fprintf(dot_file, "  node%d [label=\"%s\", fillcolor=\"%s\"%s];\n",
            *node_counter, label ? label : "ERROR", final_color, extra_attrs);

    free(label); // Libérer la mémoire allouée par get_node_label
}

/**
 * Retourne une couleur pour un type de nœud
 *
 * @param type Type de nœud
 * @return Chaîne représentant la couleur en hexadécimal
 */
char    *get_node_color(enum node_type type)
{
    switch (type)
    {
        case NODE_CMD:
            return "#98FB98"; // Vert pâle
        case NODE_PIPE:
            return "#87CEFA"; // Bleu clair
        case NODE_AND:
            return "#FFA07A"; // Orange clair
        case NODE_OR:
            return "#FFD700"; // Or
        case NODE_OPEN_PARENTHESIS:
        case NODE_CLOSE_PARENTHESIS:
            return "#D8BFD8"; // Violet pâle
        default:
            return "#FFFFFF"; // Blanc
    }
}

/**
 * Génère une étiquette pour un nœud
 *
 * @param node Nœud pour lequel générer une étiquette
 * @return Chaîne allouée dynamiquement contenant l'étiquette
 */
char    *get_node_label(t_ast_node *node)
{
    char *label;
    char redir_part[512] = ""; // Augmenté la taille pour plus de détails
    int has_redirs = 0;

    if (!node)
        return strdup("NULL");

    // Allouer assez d'espace pour l'étiquette la plus longue possible
    label = (char *)malloc(1024); // Augmenté pour permettre plus de détails
    if (!label)
        return strdup("ERROR");

    switch (node->type)
    {
        case NODE_CMD:
            if (node->cmd)
            {
                // Détailler toutes les redirections
                if (node->cmd->redirs)
                {
                    has_redirs = 1;
                    t_redir *redir = node->cmd->redirs;
                    char temp_redir[256];

                    // En-tête des redirections avec séparateur visuel
                    strcpy(redir_part, "\\n\\n━━━━ REDIRECTIONS ━━━━");

                    // Parcourir toutes les redirections
                    int redir_count = 0;
                    while (redir)
                    {
                        char redir_symbol[5] = "";
                        char redir_type[20] = "";
                        char redir_color[20] = "";

                        // Obtenir le symbole, le type et la couleur de redirection
                        switch (redir->type)
                        {
                            case REDIR_IN:
                                strcpy(redir_symbol, "<");
                                strcpy(redir_type, "ENTRÉE");
                                strcpy(redir_color, "green");
                                break;
                            case REDIR_OUT:
                                strcpy(redir_symbol, ">");
                                strcpy(redir_type, "SORTIE");
                                strcpy(redir_color, "red");
                                break;
                            case REDIR_APPEND:
                                strcpy(redir_symbol, ">>");
                                strcpy(redir_type, "APPEND");
                                strcpy(redir_color, "orange");
                                break;
                            case REDIR_HEREDOC:
                                strcpy(redir_symbol, "<<");
                                strcpy(redir_type, "HEREDOC");
                                strcpy(redir_color, "purple");
                                break;
                            default:
                                strcpy(redir_symbol, "?");
                                strcpy(redir_type, "INCONNU");
                                strcpy(redir_color, "gray");
                                break;
                        }

                        // Formater cette redirection avec des séparateurs
                        if (redir_count > 0)
                            strcat(redir_part, "\\n──────────────────");

                        // Formater cette redirection avec un format plus détaillé
                        snprintf(temp_redir, sizeof(temp_redir),
                                "\\n• Type: %s (%s)\\n• Fichier: %s",
                                redir_type, redir_symbol,
                                redir->file_or_delimiter ? redir->file_or_delimiter : "(none)");

                        // Ajouter à la chaîne des redirections
                        strcat(redir_part, temp_redir);

                        redir = redir->next;
                        redir_count++;
                    }
                }

                // Commande avec arguments
                if (node->cmd->command && node->cmd->command[0])
                {
                    // Compter et formater les arguments
                    int args_count = 0;
                    char args_str[512] = "";

                    // Compter les arguments
                    while (node->cmd->command[args_count])
                        args_count++;

                    // Afficher tous les arguments
                    if (args_count > 1) // Si plus d'un argument (pas juste la commande)
                    {
                        strcat(args_str, "\\n\\n━━━━ ARGUMENTS ━━━━");
                        for (int i = 1; i < args_count; i++) // Commence à 1 pour sauter la commande
                        {
                            char arg_temp[256];
                            snprintf(arg_temp, sizeof(arg_temp), "\\n[%d]: %s",
                                    i, node->cmd->command[i]);
                            strcat(args_str, arg_temp);
                        }
                    }

                    // Le titre affiche le nom de la commande principale avec un style distinctif
                    char title[256];
                    if (node->subshell)
                        snprintf(title, sizeof(title), "COMMANDE (subshell)\\n≻ %s ≺",
                                node->cmd->command[0]);
                    else
                        snprintf(title, sizeof(title), "COMMANDE\\n≻ %s ≺",
                                node->cmd->command[0]);

                    // Mettre tout ensemble
                    snprintf(label, 1024, "%s%s%s",
                            title,
                            args_str,
                            has_redirs ? redir_part : "");
                }
                else
                {
                    // Commande vide ou opérateur
                    if (node->subshell)
                        snprintf(label, 1024, "COMMANDE (subshell)\\n(Vide)%s",
                                has_redirs ? redir_part : "");
                    else
                        snprintf(label, 1024, "COMMANDE\\n(Vide)%s",
                                has_redirs ? redir_part : "");
                }
            }
            else
            {
                // Nœud sans commande
                snprintf(label, 1024, "COMMANDE\\n(Aucune cmd)");
            }
            break;
        case NODE_PIPE:
            if (node->subshell)
                snprintf(label, 1024, "PIPE | (subshell)");
            else
                snprintf(label, 1024, "PIPE |");
            break;
        case NODE_AND:
            if (node->subshell)
                snprintf(label, 1024, "AND && (subshell)");
            else
                snprintf(label, 1024, "AND &&");
            break;
        case NODE_OR:
            if (node->subshell)
                snprintf(label, 1024, "OR || (subshell)");
            else
                snprintf(label, 1024, "OR ||");
            break;
        case NODE_OPEN_PARENTHESIS:
            snprintf(label, 1024, "PARENTHÈSE (");
            break;
        case NODE_CLOSE_PARENTHESIS:
            snprintf(label, 1024, "PARENTHÈSE )");
            break;
        default:
            snprintf(label, 1024, "INCONNU (%d)", node->type);
    }

    return label;
}
