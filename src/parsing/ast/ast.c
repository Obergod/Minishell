/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 15:20:50 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/02 18:02:20 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/ast.h"

t_ast_node *init_ast_node(enum node_type type, t_cmd *cmd, t_minishell *minishell)
{
	t_ast_node *node;

	node = gc_malloc(sizeof(t_ast_node), minishell->gc);
	if (!node)
	{
		printf("[DEBUG-AST] Échec d'allocation pour init_ast_node\n");
		return (NULL);
	}
	node->type = type;
	node->cmd = cmd;
	node->subshell = 0;
	node->left = NULL;
	node->right = NULL;
	printf("[DEBUG-AST] Création nœud type=%d, cmd=%s\n",
		type, (cmd && cmd->command_raw) ? cmd->command_raw : "NULL");
	return (node);
}

static int is_logic_op(enum e_logic_operator_type op)
{
	return (op == AND || op == OR || op == OPEN_PARENTHESIS
			|| op == CLOSE_PARENTHESIS);
}

// Déclaration préalable de build_ast_recursive pour éviter l'erreur de compilation
t_ast_node *build_ast_recursive(t_cmd **cmd_list, t_minishell *minishell,
								int *parenthesis_count);

static int handle_parenthesis(t_cmd **cmd_list, t_ast_node **ast,
							t_minishell *minishell, int *parenthesis_count)
{
	t_ast_node *subshell_node;
	t_cmd *tmp;

	printf("[DEBUG-AST] handle_parenthesis: type=%d, count=%d\n",
		(*cmd_list)->logic_operator_type, *parenthesis_count);

	if ((*cmd_list)->logic_operator_type == OPEN_PARENTHESIS)
	{
		(*parenthesis_count)++;
		printf("[DEBUG-AST] Parenthèse ouvrante trouvée, nouveau count=%d\n", *parenthesis_count);

		// Passer au prochain élément et construire le sous-arbre
		tmp = *cmd_list;
		*cmd_list = (*cmd_list)->next;
		if (!*cmd_list)
		{
			printf("[DEBUG-AST] ERREUR: Parenthèse ouvrante sans contenu\n");
			return (0); // Erreur: parenthèse ouvrante sans contenu
		}

		printf("[DEBUG-AST] Construction du sous-arbre après parenthèse ouvrante\n");
		// Créer un nouveau nœud pour la sous-expression
		subshell_node = build_ast_recursive(cmd_list, minishell, parenthesis_count);
		if (!subshell_node)
		{
			printf("[DEBUG-AST] ERREUR: Échec de construction du sous-arbre\n");
			return (0);
		}

		subshell_node->subshell = 1;
		*ast = subshell_node;
		printf("[DEBUG-AST] Sous-arbre construit avec succès, subshell=1\n");

		// Vérifier si on a géré la parenthèse fermante correctement
		// Il n'est pas nécessaire d'avoir *parenthesis_count < 0 comme condition d'erreur,
		// car ce cas est déjà géré dans la partie CLOSE_PARENTHESIS

		return (1);
	}
	else if ((*cmd_list)->logic_operator_type == CLOSE_PARENTHESIS)
	{
		(*parenthesis_count)--;
		printf("[DEBUG-AST] Parenthèse fermante trouvée, nouveau count=%d\n", *parenthesis_count);

		if (*parenthesis_count < 0)
		{
			printf("[DEBUG-AST] ERREUR: Parenthèse fermante sans ouvrante correspondante\n");
			return (0); // Erreur: parenthèse fermante sans ouvrante correspondante
		}

		// Avancer au-delà de la parenthèse fermante
		*cmd_list = (*cmd_list)->next;
		return (2); // Code spécial pour indiquer qu'on a trouvé une parenthèse fermante
	}

	return (1);
}

static t_ast_node *build_expr(t_cmd **cmd_list, t_minishell *minishell,
							int *parenthesis_count)
{
	t_ast_node *node;
	int result;

	if (!cmd_list || !*cmd_list)
	{
		printf("[DEBUG-AST] build_expr: cmd_list est NULL\n");
		return (NULL);
	}

	printf("[DEBUG-AST] build_expr: cmd=%s, op_type=%d\n",
		((*cmd_list)->command_raw) ? (*cmd_list)->command_raw : "NULL",
		(*cmd_list)->logic_operator_type);

	// Gérer explicitement les parenthèses
	if ((*cmd_list)->logic_operator_type == OPEN_PARENTHESIS ||
		(*cmd_list)->logic_operator_type == CLOSE_PARENTHESIS)
	{
		printf("[DEBUG-AST] Traitement d'une parenthèse\n");
		result = handle_parenthesis(cmd_list, &node, minishell, parenthesis_count);
		if (result == 0)
		{
			printf("[DEBUG-AST] ERREUR dans le traitement des parenthèses\n");
			return (NULL); // Erreur dans les parenthèses
		}
		else if (result == 2)
		{
			printf("[DEBUG-AST] Parenthèse fermante trouvée, fin de sous-expression\n");
			return (NULL); // Parenthèse fermante - signal spécial
		}
		printf("[DEBUG-AST] Parenthèse traitée avec succès\n");
		return (node);
	}

	// Créer un nœud pour une commande simple
	printf("[DEBUG-AST] Création d'un nœud pour commande simple\n");
	node = init_ast_node(NODE_CMD, *cmd_list, minishell);
	if (!node)
	{
		printf("[DEBUG-AST] ERREUR: Échec de création du nœud commande\n");
		return (NULL);
	}

	// Avancer à la commande suivante
	*cmd_list = (*cmd_list)->next;

	return (node);
}

t_ast_node *build_ast_recursive(t_cmd **cmd_list, t_minishell *minishell,
								int *parenthesis_count)
{
	t_ast_node *left;
	t_ast_node *root;
	enum node_type op_type;
	t_cmd *op_cmd;

	if (!*cmd_list)
	{
		printf("[DEBUG-AST] build_ast_recursive: cmd_list est NULL\n");
		return (NULL);
	}

	printf("[DEBUG-AST] build_ast_recursive: cmd=%s, op_type=%d, count=%d\n",
		((*cmd_list)->command_raw) ? (*cmd_list)->command_raw : "NULL",
		(*cmd_list)->logic_operator_type, *parenthesis_count);

	// Traiter immédiatement le cas d'une parenthèse fermante
	if ((*cmd_list)->logic_operator_type == CLOSE_PARENTHESIS)
	{
		printf("[DEBUG-AST] Détection directe d'une parenthèse fermante\n");
		(*parenthesis_count)--;
		printf("[DEBUG-AST] Parenthèse fermante trouvée (directe), nouveau count=%d\n", *parenthesis_count);
		if (*parenthesis_count < 0)
		{
			printf("[DEBUG-AST] ERREUR: Parenthèse fermante sans ouvrante correspondante\n");
			return (NULL);
		}
		*cmd_list = (*cmd_list)->next;
		return (NULL); // Signal pour remonter d'un niveau de récursion
	}

	// Construire d'abord le côté gauche de l'expression
	left = build_expr(cmd_list, minishell, parenthesis_count);
	if (!left)
	{
		// Vérifier si c'est à cause d'une parenthèse fermante
		if (*parenthesis_count >= 0)
		{
			printf("[DEBUG-AST] ERREUR: Échec de construction du côté gauche\n");
		}
		return (NULL);
	}

	// Si plus de commandes
	if (!*cmd_list)
	{
		printf("[DEBUG-AST] Fin de construction (plus de commandes)\n");
		return (left);
	}

	// Si on a trouvé une parenthèse fermante, vérifions-la explicitement
	if ((*cmd_list)->logic_operator_type == CLOSE_PARENTHESIS)
	{
		printf("[DEBUG-AST] Détection d'une parenthèse fermante après construction gauche\n");
		(*parenthesis_count)--;
		printf("[DEBUG-AST] Parenthèse fermante trouvée, nouveau count=%d\n", *parenthesis_count);
		if (*parenthesis_count < 0)
		{
			printf("[DEBUG-AST] ERREUR: Parenthèse fermante sans ouvrante correspondante\n");
			return (NULL);
		}
		*cmd_list = (*cmd_list)->next;
		return (left);
	}

	printf("[DEBUG-AST] Prochain élément: cmd=%s, op_type=%d\n",
		(*cmd_list && (*cmd_list)->command_raw) ? (*cmd_list)->command_raw : "NULL",
		(*cmd_list) ? (*cmd_list)->logic_operator_type : -1);

	// Si l'opérateur est AND ou OR, c'est un nœud binaire
	if ((*cmd_list)->logic_operator_type == AND || (*cmd_list)->logic_operator_type == OR)
	{
		op_type = ((*cmd_list)->logic_operator_type == AND) ? NODE_AND : NODE_OR;
		op_cmd = *cmd_list;

		printf("[DEBUG-AST] Opérateur %s trouvé\n", (op_type == NODE_AND) ? "AND" : "OR");

		// Avancer à la commande suivante après l'opérateur
		*cmd_list = (*cmd_list)->next;
		if (!*cmd_list)
		{
			printf("[DEBUG-AST] ERREUR: Opérateur sans opérande droite\n");
			return (NULL); // Erreur: opérateur sans opérande droite
		}

		// Créer le nœud racine avec l'opérateur
		root = init_ast_node(op_type, op_cmd, minishell);
		if (!root)
		{
			printf("[DEBUG-AST] ERREUR: Échec de création du nœud opérateur\n");
			return (NULL);
		}

		root->left = left;
		printf("[DEBUG-AST] Construction récursive du côté droit de l'opérateur\n");
		// Construire récursivement le côté droit
		root->right = build_ast_recursive(cmd_list, minishell, parenthesis_count);
		if (!root->right)
		{
			printf("[DEBUG-AST] ERREUR: Échec de construction du côté droit\n");
			// Nettoyage si nécessaire (géré par le garbage collector)
			return (NULL);
		}

		printf("[DEBUG-AST] Nœud opérateur %s construit avec succès\n",
			(op_type == NODE_AND) ? "AND" : "OR");
		return (root);
	}

	// Si c'est un pipe implicite
	if (!is_logic_op((*cmd_list)->logic_operator_type))
	{
		printf("[DEBUG-AST] Pipe implicite détecté\n");
		root = init_ast_node(NODE_PIPE, NULL, minishell);
		if (!root)
		{
			printf("[DEBUG-AST] ERREUR: Échec de création du nœud pipe\n");
			return (NULL);
		}

		root->left = left;
		printf("[DEBUG-AST] Construction récursive du côté droit du pipe\n");
		// Construire récursivement le côté droit (pipe implicite)
		root->right = build_ast_recursive(cmd_list, minishell, parenthesis_count);
		if (!root->right)
		{
			printf("[DEBUG-AST] ERREUR: Échec de construction du côté droit du pipe\n");
			// Nettoyage si nécessaire (géré par le garbage collector)
			return (NULL);
		}

		printf("[DEBUG-AST] Nœud pipe construit avec succès\n");
		return (root);
	}

	printf("[DEBUG-AST] Retour du côté gauche uniquement (opérateur non géré: %d)\n",
		(*cmd_list)->logic_operator_type);
	return (left);
}

t_ast_node *build_ast(t_cmd *cmd_list, t_minishell *minishell)
{
	int parenthesis_count;
	t_ast_node *root;

	printf("\n[DEBUG-AST] ===== DÉBUT DE CONSTRUCTION AST =====\n");

	if (!cmd_list)
	{
		printf("[DEBUG-AST] Liste de commandes vide\n");
		return (NULL);
	}

	// Afficher la liste des commandes pour débogage
	t_cmd *debug_cmd = cmd_list;
	int cmd_idx = 0;
	printf("[DEBUG-AST] Liste des commandes à traiter:\n");
	while (debug_cmd)
	{
		printf("[DEBUG-AST]   Cmd %d: '%s', op=%d\n",
			cmd_idx++,
			debug_cmd->command_raw ? debug_cmd->command_raw : "NULL",
			debug_cmd->logic_operator_type);
		debug_cmd = debug_cmd->next;
	}

	// Vérifier si la première commande est une parenthèse ouvrante
	// Cas spécial: si la première commande est une parenthèse ouvrante mais n'a pas de contenu
	if (cmd_list->logic_operator_type == OPEN_PARENTHESIS && !cmd_list->command_raw && !cmd_list->next)
	{
		printf("[DEBUG-AST] ERREUR: Parenthèse ouvrante sans contenu\n");
		return (NULL);
	}

	parenthesis_count = 0;
	printf("[DEBUG-AST] Appel à build_ast_recursive\n");
	root = build_ast_recursive(&cmd_list, minishell, &parenthesis_count);

	// Vérification finale des parenthèses équilibrées
	printf("[DEBUG-AST] Vérification finale: parenthesis_count=%d\n", parenthesis_count);
	if (parenthesis_count != 0)
	{
		// Si nous avons un déséquilibre positif, il manque des parenthèses fermantes
		if (parenthesis_count > 0)
			printf("[DEBUG-AST] ERREUR: Parenthèses non équilibrées - il manque %d parenthèse(s) fermante(s)\n",
				parenthesis_count);
		else
			printf("[DEBUG-AST] ERREUR: Parenthèses non équilibrées - il y a %d parenthèse(s) fermante(s) en trop\n",
				-parenthesis_count);

		// Dans un shell interactif, nous pourrions demander plus d'input, mais ici nous échouons simplement
		return (NULL);
	}

	// Si nous sommes arrivés à la fin de la liste, mais qu'il reste encore des tokens
	// C'est qu'il y a une erreur de syntaxe
	if (cmd_list != NULL)
	{
		printf("[DEBUG-AST] ERREUR: Tokens supplémentaires après la construction de l'AST\n");
		// Afficher les tokens restants
		cmd_idx = 0;
		while (cmd_list)
		{
			printf("[DEBUG-AST]   Token restant %d: '%s', op=%d\n",
				cmd_idx++,
				cmd_list->command_raw ? cmd_list->command_raw : "NULL",
				cmd_list->logic_operator_type);
			cmd_list = cmd_list->next;
		}
		// On permet de continuer car c'est peut-être juste un problème de parsing
	}

	if (root)
		printf("[DEBUG-AST] AST construit avec succès\n");
	else
		printf("[DEBUG-AST] ÉCHEC de construction de l'AST\n");

	printf("[DEBUG-AST] ===== FIN DE CONSTRUCTION AST =====\n\n");
	return (root);
}

void print_ast(t_ast_node *node, int depth)
{
	int i;

	if (!node)
		return;

	for (i = 0; i < depth; i++)
		printf("  ");

	switch (node->type)
	{
		case NODE_CMD:
			printf("CMD: %s\n", node->cmd->command_raw);
			break;
		case NODE_PIPE:
			printf("PIPE\n");
			break;
		case NODE_AND:
			printf("AND\n");
			break;
		case NODE_OR:
			printf("OR\n");
			break;
		default:
			printf("UNKNOWN NODE TYPE\n");
	}

	if (node->subshell)
	{
		for (i = 0; i < depth; i++)
			printf("  ");
		printf("(SUBSHELL)\n");
	}

	print_ast(node->left, depth + 1);
	print_ast(node->right, depth + 1);
}
