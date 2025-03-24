/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 15:47:46 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/24 19:14:23 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/ast.h"
#include <linux/limits.h>
#include <string.h>

static enum node_type convert_operator_type_to_node_type(enum e_logic_operator_type t)
{
	if (t == AND)
		return (NODE_AND);
	else if (t == OR)
		return (NODE_OR);
	else if (t == OPEN_PARENTHESIS)
		return (NODE_OPEN_PARENTHESIS);
	else if (t == CLOSE_PARENTHESIS)
		return (NODE_CLOSE_PARENTHESIS);
	else
		return (NODE_NONE);
}

static t_ast_node *new_node_ast(t_cmd *cmd, enum node_type type, t_minishell *minishell)
{
	t_ast_node *node;

	node = gc_malloc(sizeof(t_ast_node), minishell->gc);
	if (!node)
		return (NULL);
	node->cmd = cmd;
	node->left = NULL;
	node->right = NULL;
	node->type = type;
	node->subshell = 0;
	return (node);
}

static t_ast_node *new_node_cmd_ast(t_cmd *cmd, t_minishell *minishell)
{
	t_ast_node *node;

	node = gc_malloc(sizeof(t_ast_node), minishell->gc);
	if (!node)
		return (NULL);
	node->cmd = cmd;
	node->left = NULL;
	node->right = NULL;
	node->type = NODE_CMD;
	node->subshell = 0;
	return (node);
}
/*
enum	e_logic_operator_type
{
	NONE,
	AND,
	OR,
	OPEN_PARENTHESIS,
	CLOSE_PARENTHESIS
};
*/
static int find_operator_ast(t_cmd *cmd, enum node_type *operator_type)
{
	int in_parenthesis;
	t_cmd *cmd_cpy;
	int position;

	in_parenthesis = 0;
	*operator_type = NODE_NONE;
	cmd_cpy = cmd;
	position = 0;

	// Premier passage: recherche d'opérateurs AND et OR
	while (cmd_cpy)
	{
		if (cmd_cpy->logic_operator_type == OPEN_PARENTHESIS)
			in_parenthesis++;
		else if (cmd_cpy->logic_operator_type == CLOSE_PARENTHESIS)
			in_parenthesis--;
		else if (in_parenthesis == 0 && (cmd_cpy->logic_operator_type == AND || cmd_cpy->logic_operator_type == OR))
		{
			*operator_type = convert_operator_type_to_node_type(cmd_cpy->logic_operator_type);
			return (position);
		}
		cmd_cpy = cmd_cpy->next;
		position++;
	}
	return (position);
}

//cmd -> cmd2 -> cmd3 -> cmd4
//cmd -> cmd2 | cmd3->cmd4

//target
//tmp = target->next;
//target->next = NULL

t_cmd *duplicate_cmd(t_cmd *cmd, t_minishell *minishell)
{
	t_cmd *cmd_new_head;
	t_cmd *current;
	t_cmd *tmp;

	if (!cmd)
		return (NULL);

	// Allocation du premier nœud
	cmd_new_head = gc_malloc(sizeof(t_cmd), minishell->gc);
	if (!cmd_new_head)
		return (NULL);

	// Initialisation sécurisée avec des valeurs par défaut
	cmd_new_head->command_raw = NULL;
	cmd_new_head->command = NULL;
	cmd_new_head->_arg_capacity = 0;
	cmd_new_head->_arg_count = 0;
	cmd_new_head->logic_operator_type = NONE;
	cmd_new_head->redirs = NULL;
	cmd_new_head->next = NULL;

	// Initialiser la tête avec les valeurs de cmd si elles existent
	if (cmd->command_raw)
		cmd_new_head->command_raw = gc_strdup(cmd->command_raw, minishell->gc);

	if (cmd->command)
		cmd_new_head->command = gc_strdup_array(cmd->command, minishell->gc);

	cmd_new_head->_arg_capacity = cmd->_arg_capacity;
	cmd_new_head->_arg_count = cmd->_arg_count;
	cmd_new_head->logic_operator_type = cmd->logic_operator_type;
	cmd_new_head->redirs = cmd->redirs;
	cmd_new_head->next = NULL;

	tmp = cmd_new_head;
	cmd = cmd->next;

	// Dupliquer le reste de la chaîne
	while (cmd)
	{
		current = gc_malloc(sizeof(t_cmd), minishell->gc);
		if (!current)
			return (cmd_new_head);
		current->command_raw = NULL;
		current->command = NULL;
		current->_arg_capacity = 0;
		current->_arg_count = 0;
		current->logic_operator_type = NONE;
		current->redirs = NULL;
		current->next = NULL;
		if (cmd->command_raw)
			current->command_raw = gc_strdup(cmd->command_raw, minishell->gc);
		if (cmd->command)
			current->command = gc_strdup_array(cmd->command, minishell->gc);
		current->_arg_capacity = cmd->_arg_capacity;
		current->_arg_count = cmd->_arg_count;
		current->logic_operator_type = cmd->logic_operator_type;
		current->redirs = cmd->redirs;
		current->next = NULL;
		tmp->next = current;
		tmp = current;
		cmd = cmd->next;
	}
	return (cmd_new_head);
}

static t_cmd * bloc_parenthesis_ast(t_cmd *cmd, int *bloc_parenthese, t_minishell *minishell)
{
	int in_parenthesis;
	int size_parenthesis;
	t_cmd *new;
	t_cmd *current;
	t_cmd *content_start;

	if (!cmd || !bloc_parenthese)
	{
		if (bloc_parenthese)
			*bloc_parenthese = -1;
		return (NULL);
	}

	in_parenthesis = 0;
	size_parenthesis = 0;

	new = duplicate_cmd(cmd, minishell);
	if (!new)
	{
		*bloc_parenthese = -1;
		return (NULL);
	}
	current = new;
	content_start = NULL;
	while (current)
	{
		if (current->logic_operator_type == OPEN_PARENTHESIS && in_parenthesis == 0)
		{
			in_parenthesis++;
			content_start = current->next;
			break;
		}
		current = current->next;
	}
	if (!content_start)
	{
		*bloc_parenthese = -1;
		return (NULL);
	}
	current = content_start;
	t_cmd *previous = NULL;

	while (current)
	{
		if (current->logic_operator_type == OPEN_PARENTHESIS)
		{
			in_parenthesis++;
			printf("DEBUG bloc_parenthesis: Parenthèse imbriquée ouverte, niveau=%d\n", in_parenthesis);
		}
		else if (current->logic_operator_type == CLOSE_PARENTHESIS)
		{
			in_parenthesis--;
			printf("DEBUG bloc_parenthesis: Parenthèse fermée, niveau=%d\n", in_parenthesis);

			if (in_parenthesis == 0)
			{
				// On a trouvé la fermeture correspondante
				if (previous)
					previous->next = NULL; // Détacher la fin

				*bloc_parenthese = size_parenthesis;
				printf("DEBUG bloc_parenthesis: Contenu trouvé, taille=%d\n", size_parenthesis);
				t_cmd *result = duplicate_cmd(content_start, minishell);
				printf("DEBUG bloc_parenthesis: Contenu=%p\n", (void*)result);
				return result;
			}
		}

		size_parenthesis++;
		previous = current;
		current = current->next;
	}

	printf("DEBUG bloc_parenthesis: Parenthèse fermante non trouvée\n");
	*bloc_parenthese = -1;
	return (NULL);
}

//cmd0->echo, cmd->1 salut, cmd->2 &&, cmd3-> grep
static t_cmd * find_command_before_operator(t_cmd *cmd, int position, t_minishell *minishell)
{
	t_cmd *new;
	t_cmd *current;
	int i;

	printf("DEBUG find_before: Entrée avec cmd=%p, position=%d\n", (void*)cmd, position);

	// Vérifier les paramètres
	if (!cmd)
	{
		printf("DEBUG find_before: Sortie avec NULL - cmd est NULL\n");
		return (NULL);
	}

	// Cas spécial: position hors limites ou zéro
	if (position <= 0)
	{
		printf("DEBUG find_before: Sortie avec NULL - position invalide %d\n", position);
		return (NULL);
	}

	// Compter le nombre de commandes dans la chaîne
	int cmd_count = 0;
	t_cmd *tmp = cmd;
	while (tmp)
	{
		cmd_count++;
		tmp = tmp->next;
	}
	printf("DEBUG find_before: Nombre total de commandes: %d\n", cmd_count);

	// Vérifier si position est valide
	if (position > cmd_count)
	{
		printf("DEBUG find_before: Position %d > nombre de commandes %d\n", position, cmd_count);
		position = cmd_count;
	}

	printf("DEBUG find_before: Duplication commande\n");
	new = duplicate_cmd(cmd, minishell);
	if (!new)
	{
		printf("DEBUG find_before: Sortie avec NULL - échec duplication\n");
		return (NULL);
	}

	if (position == 1)
	{
		printf("DEBUG find_before: Position = 1, création commande unique\n");
		t_cmd *single_cmd = duplicate_cmd(new, minishell);
		if (single_cmd)
			single_cmd->next = NULL; // Isoler cette commande
		printf("DEBUG find_before: Retour commande unique %p\n", (void*)single_cmd);
		return single_cmd;
	}

	// Trouver le point où nous devons couper la liste
	current = new;
	i = 1;

	printf("DEBUG find_before: Parcours jusqu'à position-1 (%d)\n", position-1);
	while (current && i < position)
	{
		if (i == position - 1)
		{
			// On est arrivé à l'élément juste avant la position cible
			if (current->next)
			{
				printf("DEBUG find_before: Troncature à position %d\n", i);
				current->next = NULL; // Couper la liste ici
			}
			break;
		}

		i++;
		current = current->next;
		printf("DEBUG find_before: Avance à i=%d, current=%p\n", i, (void*)current);
	}

	printf("DEBUG find_before: Sortie avec new=%p\n", (void*)new);
	return (new);
}

static t_cmd *find_command_after_operator(t_cmd *cmd, int position, t_minishell *minishell)
{
	t_cmd *new;
	t_cmd *current;
	int i;

	printf("DEBUG find_after: Entrée avec cmd=%p, position=%d\n", (void*)cmd, position);

	// Vérifier les paramètres
	if (!cmd)
	{
		printf("DEBUG find_after: Sortie avec NULL - cmd est NULL\n");
		return (NULL);
	}

	// Compter le nombre de commandes dans la chaîne
	int cmd_count = 0;
	t_cmd *tmp = cmd;
	while (tmp)
	{
		cmd_count++;
		tmp = tmp->next;
	}
	printf("DEBUG find_after: Nombre total de commandes: %d\n", cmd_count);

	// Vérifier si position est valide
	if (position >= cmd_count || position < 0)
	{
		printf("DEBUG find_after: Position %d hors limites (cmd_count=%d)\n", position, cmd_count);
		return (NULL);
	}

	printf("DEBUG find_after: Duplication commande\n");
	new = duplicate_cmd(cmd, minishell);
	if (!new)
	{
		printf("DEBUG find_after: Sortie avec NULL - échec duplication\n");
		return (NULL);
	}

	current = new;
	i = 0;

	printf("DEBUG find_after: Recherche de la position %d\n", position);
	// Trouver la commande à la position spécifiée
	while (current && i < position)
	{
		current = current->next;
		i++;
	}

	if (!current || !current->next)
	{
		printf("DEBUG find_after: Pas de commande après position %d\n", position);
		return NULL;
	}

	// Récupérer la commande juste après la position (l'opérateur)
	t_cmd *result = duplicate_cmd(current->next, minishell);
	printf("DEBUG find_after: Résultat=%p, commande_raw=%s\n",
		(void*)result,
		result && result->command_raw ? result->command_raw : "NULL");

	return result;
}

static int is_pipe_command_group(t_cmd *cmd)
{
    // Si nous avons deux commandes consécutives sans opérateur logique
    return (cmd && cmd->next &&
            cmd->logic_operator_type == NONE &&
            cmd->next->logic_operator_type != AND &&
            cmd->next->logic_operator_type != OR &&
            cmd->next->logic_operator_type != CLOSE_PARENTHESIS);
}

t_ast_node *build_ast(t_cmd *cmd, t_minishell *minishell)
{
	t_ast_node *node;
	t_ast_node *sous_arbre;
	enum node_type operator;
	int bloc_parenthese;
	int position;
	t_cmd *bloc_cmd;

	printf("DEBUG AST BUILD: Entrée avec cmd=%p\n", (void*)cmd);
	if (!cmd)
	{
		printf("DEBUG AST BUILD: Sortie avec NULL - cmd est NULL\n");
		return (NULL);
	}

	// Afficher la chaîne de commandes à analyser
	t_cmd *print_cmd = cmd;
	int cmd_index = 0;
	printf("DEBUG AST BUILD: Analyse de la chaîne de commandes:\n");
	while (print_cmd)
	{
		printf("  CMD[%d]: raw=%s, op=%d\n",
			cmd_index++,
			print_cmd->command_raw ? print_cmd->command_raw : "NULL",
			print_cmd->logic_operator_type);
		print_cmd = print_cmd->next;
	}

	printf("DEBUG AST BUILD: Recherche d'opérateur logique\n");
	position = find_operator_ast(cmd, &operator);
	printf("DEBUG AST BUILD: position=%d, operator=%d\n", position, operator);

	if (operator != NODE_NONE)
	{
		printf("DEBUG AST BUILD: Création d'un nœud opérateur %d\n", operator);
		node = new_node_ast(cmd, operator, minishell);
		if (!node)
		{
			printf("DEBUG AST BUILD: Échec création nœud opérateur\n");
			return (NULL);
		}

		printf("DEBUG AST BUILD: Construction sous-arbre gauche\n");
		t_cmd *cmd_left = find_command_before_operator(cmd, position, minishell);
		printf("DEBUG AST BUILD: cmd_left=%p\n", (void*)cmd_left);
		node->left = build_ast(cmd_left, minishell);

		printf("DEBUG AST BUILD: Construction sous-arbre droit\n");
		t_cmd *cmd_right = find_command_after_operator(cmd, position, minishell);
		printf("DEBUG AST BUILD: cmd_right=%p\n", (void*)cmd_right);
		node->right = build_ast(cmd_right, minishell);

		printf("DEBUG AST BUILD: Retour nœud opérateur %p\n", (void*)node);
		return (node);
	}

	printf("DEBUG AST BUILD: Recherche de bloc parenthèse\n");
	bloc_cmd = bloc_parenthesis_ast(cmd, &bloc_parenthese, minishell);
	printf("DEBUG AST BUILD: bloc_cmd=%p, bloc_parenthese=%d\n", (void*)bloc_cmd, bloc_parenthese);

	if (bloc_parenthese != -1)
	{
		printf("DEBUG AST BUILD: Construction sous-arbre parenthésé\n");
		sous_arbre = build_ast(bloc_cmd, minishell);
		if (sous_arbre)
		{
			printf("DEBUG AST BUILD: Sous-arbre parenthésé créé, marquage subshell=1\n");
			sous_arbre->subshell = 1;
		}
		else
			printf("DEBUG AST BUILD: Échec création sous-arbre parenthésé\n");

		printf("DEBUG AST BUILD: Retour sous-arbre parenthésé %p\n", (void*)sous_arbre);
		return (sous_arbre);
	}

	// Vérifier si nous avons un groupe de commandes reliées par des pipes implicites
	if (is_pipe_command_group(cmd))
	{
		printf("DEBUG AST BUILD: Détection de commandes séparées par pipe implicite\n");
		printf("DEBUG AST BUILD: cmd1=%s, cmd2=%s\n",
			cmd->command_raw ? cmd->command_raw : "NULL",
			cmd->next->command_raw ? cmd->next->command_raw : "NULL");

		// Créer un nœud pipe
		node = new_node_ast(cmd, NODE_PIPE, minishell);
		if (!node)
		{
			printf("DEBUG AST BUILD: Échec création nœud pipe\n");
			return (NULL);
		}

		// Commande de gauche
		printf("DEBUG AST BUILD: Création sous-arbre gauche pour le pipe\n");
		t_cmd *left_cmd = duplicate_cmd(cmd, minishell);
		if (left_cmd)
			left_cmd->next = NULL; // Isoler la première commande

		node->left = new_node_cmd_ast(left_cmd, minishell);
		if (!node->left)
			printf("DEBUG AST BUILD: Échec création nœud de gauche (commande)\n");

		// Commande de droite (peut être un autre pipe ou une commande simple)
		printf("DEBUG AST BUILD: Construction sous-arbre droit du pipe\n");
		node->right = build_ast(cmd->next, minishell);
		if (!node->right)
			printf("DEBUG AST BUILD: Échec création nœud de droite\n");

		printf("DEBUG AST BUILD: Retour nœud pipe %p\n", (void*)node);
		return (node);
	}

	// Cas simple: commande unique
	if (cmd->command != NULL)
	{
		printf("DEBUG AST BUILD: Création nœud commande simple pour '%s'\n",
			cmd->command_raw ? cmd->command_raw : "NULL");
		node = new_node_cmd_ast(cmd, minishell);
		printf("DEBUG AST BUILD: Retour nœud commande %p\n", (void*)node);
		return (node);
	}

	printf("DEBUG AST BUILD: Aucun motif reconnu, création nœud vide\n");
	node = new_node_cmd_ast(cmd, minishell);
	printf("DEBUG AST BUILD: Retour nœud vide %p\n", (void*)node);
	return (node);
}
