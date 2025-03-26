/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:11:42 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/25 16:43:29 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/ast.h"
#include <string.h>

/**
 * Convertit un type d'opérateur logique en type de noeud correspondant
 */
static enum node_type	convert_operator_type(enum e_logic_operator_type type)
{
	if (type == AND)
		return (NODE_AND);
	else if (type == OR)
		return (NODE_OR);
	else if (type == OPEN_PARENTHESIS)
		return (NODE_OPEN_PARENTHESIS);
	else if (type == CLOSE_PARENTHESIS)
		return (NODE_CLOSE_PARENTHESIS);
	return (NODE_NONE);
}

/**
 * Crée un nouveau noeud dans l'AST
 */
t_ast_node	*create_node(enum node_type type, t_minishell *minishell)
{
	t_ast_node	*node;

	node = gc_malloc(sizeof(t_ast_node), minishell->gc);
	if (!node)
		return (NULL);
	node->type = type;
	node->cmd = NULL;
	node->subshell = 0;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

/**
 * Crée un noeud de commande dans l'AST
 */
t_ast_node	*create_command_node(t_cmd *cmd, t_minishell *minishell)
{
	t_ast_node	*node;

	node = create_node(NODE_CMD, minishell);
	if (!node)
		return (NULL);
	node->cmd = cmd;
	return (node);
}

/**
 * Duplique une liste de commandes
 */
static t_cmd	*duplicate_cmd_list(t_cmd *cmd, t_minishell *minishell)
{
	t_cmd	*new_head;
	t_cmd	*current;
	t_cmd	*tmp;
	t_cmd	*to_copy;

	if (!cmd)
		return (NULL);
	new_head = NULL;
	to_copy = cmd;
	while (to_copy)
	{
		current = new_cmd(minishell);
		if (!current)
			return (new_head);
		if (to_copy->command_raw)
			current->command_raw = gc_strdup(to_copy->command_raw, minishell->gc);
		if (to_copy->command)
			current->command = gc_strdup_array(to_copy->command, minishell->gc);
		current->_arg_capacity = to_copy->_arg_capacity;
		current->_arg_count = to_copy->_arg_count;
		current->logic_operator_type = to_copy->logic_operator_type;
		current->redirs = to_copy->redirs;
		if (!new_head)
			new_head = current;
		else
		{
			tmp = new_head;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = current;
		}
		to_copy = to_copy->next;
	}
	return (new_head);
}

/**
 * Recherche le premier opérateur logique (AND, OR) en dehors des parenthèses
 * Cette fonction est conservée pour compatibilité avec d'autres parties du code
 */
static int	find_highest_logic_operator(t_cmd *cmd, enum node_type *operator_type)
{
	int		in_parenthesis;
	t_cmd	*current;
	int		position;

	in_parenthesis = 0;
	*operator_type = NODE_NONE;
	current = cmd;
	position = 0;
	while (current)
	{
		if (current->logic_operator_type == OPEN_PARENTHESIS)
			in_parenthesis++;
		else if (current->logic_operator_type == CLOSE_PARENTHESIS)
			in_parenthesis--;
		else if (in_parenthesis == 0 && (current->logic_operator_type == AND
				|| current->logic_operator_type == OR))
		{
			*operator_type = convert_operator_type(current->logic_operator_type);
			return (position);
		}
		current = current->next;
		position++;
	}
	return (position);
}

/**
 * Vérifie si la commande est une commande nulle avec un type d'opérateur
 */
static int	is_empty_operator_cmd(t_cmd *cmd)
{
	return (cmd && (!cmd->command_raw || !cmd->command[0]) && cmd->redirs == NULL);
}

/**
 * Reconstruit la liste de commandes en ignorant les commandes vides intermédiaires
 * qui ne contiennent que des opérateurs
 */
static t_cmd	*clean_cmd_list(t_cmd *cmd_list, t_minishell *minishell)
{
	t_cmd	*result;
	t_cmd	*current;
	t_cmd	*new_cmd;
	t_cmd	*last;

	if (!cmd_list)
		return (NULL);

	result = NULL;
	last = NULL;
	current = cmd_list;

	while (current)
	{
		// On ne garde que les commandes non vides ou les opérateurs importants
		if (!is_empty_operator_cmd(current) ||
			current->logic_operator_type == OPEN_PARENTHESIS ||
			current->logic_operator_type == CLOSE_PARENTHESIS ||
			current->logic_operator_type == AND ||
			current->logic_operator_type == OR)
		{
			new_cmd = duplicate_cmd_list(current, minishell);
			if (new_cmd)
			{
				new_cmd->next = NULL;
				if (!result)
					result = new_cmd;
				else
					last->next = new_cmd;
				last = new_cmd;
			}
		}
		current = current->next;
	}

	printf("DEBUG: Liste nettoyée:\n");
	current = result;
	while (current)
	{
		printf("DEBUG:   Cmd: %s (Op: %d)\n",
			current->command_raw ? current->command_raw : "(null)",
			current->logic_operator_type);
		current = current->next;
	}

	return (result);
}

/**
 * Récupère les commandes avant l'opérateur spécifié
 */
static t_cmd	*get_commands_before_operator(t_cmd *cmd, int position,
					t_minishell *minishell)
{
	t_cmd	*new;
	t_cmd	*current;
	int		i;

	if (!cmd || position <= 0)
		return (NULL);
	new = duplicate_cmd_list(cmd, minishell);
	if (!new)
		return (NULL);
	if (position == 1)
	{
		t_cmd *single_cmd = duplicate_cmd_list(new, minishell);
		if (single_cmd)
			single_cmd->next = NULL;
		return (single_cmd);
	}
	current = new;
	i = 1;
	while (current && i < position)
	{
		if (i == position - 1)
		{
			if (current->next)
				current->next = NULL;
			break ;
		}
		i++;
		current = current->next;
	}
	return (new);
}

/**
 * Récupère les commandes après l'opérateur spécifié
 */
static t_cmd	*get_commands_after_operator(t_cmd *cmd, int position,
					t_minishell *minishell)
{
	t_cmd	*new;
	t_cmd	*current;
	int		i;

	if (!cmd)
		return (NULL);
	new = duplicate_cmd_list(cmd, minishell);
	if (!new)
		return (NULL);
	current = new;
	i = 0;
	while (current && i < position)
	{
		current = current->next;
		i++;
	}
	if (!current || !current->next)
		return (NULL);
	return (duplicate_cmd_list(current->next, minishell));
}

/**
 * Extrait la sous-liste de commandes à partir d'une position jusqu'à la fin
 * Cette fonction est conservée pour compatibilité avec d'autres parties du code
 */
static t_cmd	*get_commands_from_position(t_cmd *cmd, int position, t_minishell *minishell)
{
	t_cmd	*current;
	int		i;

	if (!cmd || position < 0)
		return (NULL);

	current = cmd;
	i = 0;

	while (current && i < position)
	{
		current = current->next;
		i++;
	}

	if (!current)
		return (NULL);

	return (duplicate_cmd_list(current, minishell));
}

/**
 * Vérifie si c'est un bloc qui démarre par une parenthèse
 */
static int is_parenthesis_block(t_cmd *cmd_list)
{
	if (!cmd_list)
		return (0);

	// On vérifie simplement si le premier élément significatif est une parenthèse ouvrante
	t_cmd *current = cmd_list;

	// Ignorer les commandes vides au début
	while (current && (!current->command_raw || !current->command_raw[0]) &&
		current->logic_operator_type != OPEN_PARENTHESIS)
		current = current->next;

	if (current && current->logic_operator_type == OPEN_PARENTHESIS)
		return (1);

	return (0);
}

/**
 * Vérifie si le nœud est entouré de parenthèses complètes
 */
static int is_complete_parenthesis_block(t_cmd *cmd_list)
{
	int depth = 0;
	int has_opening = 0;
	t_cmd *current = cmd_list;

	// Vérifier si la première commande est une parenthèse ouvrante
	if (!cmd_list || cmd_list->logic_operator_type != OPEN_PARENTHESIS)
		return (0);

	has_opening = 1;
	depth = 1;
	current = current->next;

	// Parcourir la liste pour trouver la parenthèse fermante correspondante
	while (current)
	{
		if (current->logic_operator_type == OPEN_PARENTHESIS)
			depth++;
		else if (current->logic_operator_type == CLOSE_PARENTHESIS)
		{
			depth--;
			if (depth == 0)
				break; // On a trouvé la parenthèse fermante correspondante
		}
		current = current->next;
	}

	// Si on a trouvé une parenthèse fermante correspondante à la fin
	return (has_opening && depth == 0 && current && !current->next);
}

/**
 * Extrait le contenu entre les parenthèses d'un bloc complet
 */
static t_cmd *extract_parenthesis_content(t_cmd *cmd_list, t_minishell *minishell)
{
	t_cmd *content = NULL;
	int depth = 0;

	if (!cmd_list || cmd_list->logic_operator_type != OPEN_PARENTHESIS)
		return (NULL);

	// Commencer après la parenthèse ouvrante
	t_cmd *current = cmd_list->next;
	while (current)
	{
		if (current->logic_operator_type == OPEN_PARENTHESIS)
			depth++;
		else if (current->logic_operator_type == CLOSE_PARENTHESIS)
		{
			if (depth == 0)
				break; // On a atteint la parenthèse fermante qui correspond à l'ouvrante initiale
			depth--;
		}

		// On ajoute cette commande à notre liste de contenu
		t_cmd *new_cmd = duplicate_cmd_list(current, minishell);
		if (new_cmd)
		{
			new_cmd->next = NULL;

			// Ajouter à la fin de la liste de contenu
			if (!content)
				content = new_cmd;
			else
			{
				t_cmd *last = content;
				while (last->next)
					last = last->next;
				last->next = new_cmd;
			}
		}

		current = current->next;
	}

	return (content);
}

/**
 * Vérifie si la commande actuelle est liée à la suivante par un pipe implicite
 */
static int	is_implicit_pipe(t_cmd *cmd)
{
	return (cmd && cmd->next && cmd->logic_operator_type == NONE
		&& cmd->next->logic_operator_type != AND
		&& cmd->next->logic_operator_type != OR
		&& cmd->next->logic_operator_type != CLOSE_PARENTHESIS);
}

/**
 * Applique un traitement de debug aux commandes pour montrer comment elles sont traitées
 */
static void debug_cmd_list(t_cmd *cmd_list, const char *message)
{
	printf("DEBUG: %s:\n", message);
	int i = 0;
	t_cmd *current = cmd_list;

	while (current)
	{
		printf("DEBUG:   [%d] Cmd: %s (Op: %d)\n", i++,
			current->command_raw ? current->command_raw : "(null)",
			current->logic_operator_type);
		current = current->next;
	}
}

/**
 * Vérifie si la liste de commandes contient au moins une commande de pipe implicite
 * Un pipe implicite est une commande suivie d'une autre sans opérateur
 */
static int has_implicit_pipe(t_cmd *cmd_list)
{
	t_cmd *current = cmd_list;
	int paren_level = 0;

	while (current && current->next)
	{
		// Ne pas compter les pipes à l'intérieur des parenthèses
		if (current->logic_operator_type == OPEN_PARENTHESIS)
			paren_level++;
		else if (current->logic_operator_type == CLOSE_PARENTHESIS)
		{
			if (paren_level > 0)
				paren_level--;
		}
		// Vérifier s'il y a un pipe implicite à ce niveau
		else if (paren_level == 0 &&
				current->logic_operator_type == NONE &&
				current->next->logic_operator_type == NONE)
		{
			return (1);
		}

		current = current->next;
	}

	return (0);
}

/**
 * Extrait la première paire de commandes liées par un pipe implicite
 */
static t_cmd *extract_first_pipe(t_cmd *cmd_list, t_cmd **right_part, t_minishell *minishell)
{
	t_cmd *current = cmd_list;
	t_cmd *left_part = NULL;
	int pipe_found = 0;
	int paren_level = 0;

	// Trouver le premier pipe implicite
	while (current && current->next)
	{
		if (current->logic_operator_type == OPEN_PARENTHESIS)
			paren_level++;
		else if (current->logic_operator_type == CLOSE_PARENTHESIS)
		{
			if (paren_level > 0)
				paren_level--;
		}
		else if (paren_level == 0 &&
				current->logic_operator_type == NONE &&
				current->next->logic_operator_type == NONE)
		{
			pipe_found = 1;
			break;
		}

		current = current->next;
	}

	if (!pipe_found || !current || !current->next)
	{
		*right_part = NULL;
		return NULL;
	}

	// Créer la partie gauche (un seul élément)
	left_part = duplicate_cmd_list(current, minishell);
	if (left_part)
		left_part->next = NULL;

	// Créer la partie droite (tout ce qui suit)
	*right_part = duplicate_cmd_list(current->next, minishell);

	return left_part;
}

/**
 * Crée un noeud pour un pipe implicite entre commandes
 */
static t_ast_node *create_pipe_node(t_cmd *cmd_list, t_minishell *minishell)
{
	t_ast_node *node;
	t_cmd *right_part = NULL;
	t_cmd *left_part = NULL;

	// Extraire les deux parties du pipe
	left_part = extract_first_pipe(cmd_list, &right_part, minishell);
	if (!left_part || !right_part)
		return NULL;

	// Débugger les parties extraites
	debug_cmd_list(left_part, "Partie gauche du pipe");
	debug_cmd_list(right_part, "Partie droite du pipe");

	// Créer le noeud pipe
	node = create_node(NODE_PIPE, minishell);
	if (!node)
		return NULL;

	// Partie gauche est une commande simple
	node->left = create_command_node(left_part, minishell);

	// Partie droite peut être récursivement traitée
	node->right = build_ast(right_part, minishell);

	return node;
}

/**
 * Construit l'arbre syntaxique abstrait (AST) récursivement
 */
t_ast_node	*build_ast(t_cmd *cmd_list, t_minishell *minishell)
{
	t_ast_node	*node;
	t_cmd		*block_cmd;
	t_cmd		*left_cmd;
	t_cmd		*right_cmd;
	t_cmd		*current;
	t_cmd		*cleaned_list;
	int			i;

	if (!cmd_list)
		return (NULL);

	// On nettoie la liste des commandes avant de la traiter
	cleaned_list = clean_cmd_list(cmd_list, minishell);
	if (!cleaned_list)
		return (NULL);

	// Affichage de debug pour voir la liste de commandes en entrée
	printf("\nDEBUG: ========= NOUVELLE ANALYSE AST =========\n");
	debug_cmd_list(cleaned_list, "Commandes en entrée");

	// PRIORITÉ #0: Traiter les blocs entourés de parenthèses complètes
	if (is_complete_parenthesis_block(cleaned_list))
	{
		printf("DEBUG: Traitement d'un bloc entouré de parenthèses complètes\n");
		block_cmd = extract_parenthesis_content(cleaned_list, minishell);

		if (block_cmd)
		{
			debug_cmd_list(block_cmd, "Contenu extrait des parenthèses");

			node = build_ast(block_cmd, minishell);
			if (node)
				node->subshell = 1;
			return (node);
		}
	}

	// 1. PRIORITÉ #1: Rechercher d'abord les OR au niveau le plus externe
	current = cleaned_list;
	int or_pos = -1;
	int in_paren = 0;
	int temp_pos = 0;

	printf("DEBUG: Recherche des opérateurs OR au premier niveau\n");
	while (current)
	{
		if (current->logic_operator_type == OPEN_PARENTHESIS)
			in_paren++;
		else if (current->logic_operator_type == CLOSE_PARENTHESIS)
		{
			in_paren--;
			if (in_paren < 0) // Correction pour les parenthèses non équilibrées
				in_paren = 0;
		}
		else if (in_paren == 0 && current->logic_operator_type == OR)
		{
			or_pos = temp_pos;
			printf("DEBUG: Opérateur OR trouvé à la position %d (en dehors des parenthèses)\n", or_pos);
			break;
		}
		temp_pos++;
		current = current->next;
	}

	// Si on a trouvé un OR, on le traite en priorité
	if (or_pos >= 0)
	{
		printf("DEBUG: Construction d'un noeud OR\n");
		node = create_node(NODE_OR, minishell);
		if (!node)
			return (NULL);

		t_cmd *left_cmds = get_commands_before_operator(cleaned_list, or_pos, minishell);
		t_cmd *right_cmds = get_commands_after_operator(cleaned_list, or_pos, minishell);

		debug_cmd_list(left_cmds, "Partie gauche de OR");
		debug_cmd_list(right_cmds, "Partie droite de OR");

		node->left = build_ast(left_cmds, minishell);
		node->right = build_ast(right_cmds, minishell);
		return (node);
	}

	// 2. PRIORITÉ #2: Rechercher les AND au niveau le plus externe
	current = cleaned_list;
	int and_pos = -1;
	in_paren = 0;
	temp_pos = 0;

	printf("DEBUG: Recherche des opérateurs AND au premier niveau\n");
	while (current)
	{
		if (current->logic_operator_type == OPEN_PARENTHESIS)
			in_paren++;
		else if (current->logic_operator_type == CLOSE_PARENTHESIS)
		{
			in_paren--;
			if (in_paren < 0)
				in_paren = 0;
		}
		else if (in_paren == 0 && current->logic_operator_type == AND)
		{
			and_pos = temp_pos;
			printf("DEBUG: Opérateur AND trouvé à la position %d (en dehors des parenthèses)\n", and_pos);
			break;
		}
		temp_pos++;
		current = current->next;
	}

	// Si on a trouvé un AND, on le traite
	if (and_pos >= 0)
	{
		printf("DEBUG: Construction d'un noeud AND\n");
		node = create_node(NODE_AND, minishell);
		if (!node)
			return (NULL);

		t_cmd *left_cmds = get_commands_before_operator(cleaned_list, and_pos, minishell);
		t_cmd *right_cmds = get_commands_after_operator(cleaned_list, and_pos, minishell);

		debug_cmd_list(left_cmds, "Partie gauche de AND");
		debug_cmd_list(right_cmds, "Partie droite de AND");

		node->left = build_ast(left_cmds, minishell);
		node->right = build_ast(right_cmds, minishell);
		return (node);
	}

	// 3. PRIORITÉ #3: Traiter les blocs de parenthèses avec un opérateur
	current = cleaned_list;
	if (current && current->logic_operator_type == OPEN_PARENTHESIS)
	{
		printf("DEBUG: Détection d'un bloc de parenthèses avec potentiel opérateur\n");

		// Trouver la parenthèse fermante correspondante et ce qui suit
		int depth = 1;
		t_cmd *closing_paren = NULL;
		t_cmd *after_paren = NULL;

		t_cmd *search = current->next;
		while (search)
		{
			if (search->logic_operator_type == OPEN_PARENTHESIS)
				depth++;
			else if (search->logic_operator_type == CLOSE_PARENTHESIS)
			{
				depth--;
				if (depth == 0)
				{
					closing_paren = search;
					after_paren = search->next;
					break;
				}
			}
			search = search->next;
		}

		// Si on a trouvé une parenthèse fermante et qu'elle est suivie d'un opérateur
		if (closing_paren && after_paren &&
			(after_paren->logic_operator_type == AND || after_paren->logic_operator_type == OR))
		{
			printf("DEBUG: Parenthèse suivie d'un opérateur %d\n", after_paren->logic_operator_type);

			// Extraire le contenu entre parenthèses
			t_cmd *content = NULL;
			t_cmd *last_content = NULL;
			t_cmd *current_cmd = current->next;

			while (current_cmd != closing_paren)
			{
				t_cmd *cmd_copy = duplicate_cmd_list(current_cmd, minishell);
				if (cmd_copy)
				{
					cmd_copy->next = NULL;
					if (!content)
						content = cmd_copy;
					else
					{
						last_content->next = cmd_copy;
						last_content = cmd_copy;
					}
				}
				current_cmd = current_cmd->next;
			}

			// Créer le nœud pour l'opérateur
			node = create_node(
				after_paren->logic_operator_type == AND ? NODE_AND : NODE_OR,
				minishell);

			if (!node)
				return (NULL);

			// Partie gauche : sous-arbre du contenu des parenthèses
			debug_cmd_list(content, "Contenu des parenthèses");
			node->left = build_ast(content, minishell);
			if (node->left)
				node->left->subshell = 1;

			// Partie droite : ce qui suit après l'opérateur
			if (after_paren->next)
			{
				t_cmd *right_part = duplicate_cmd_list(after_paren->next, minishell);
				debug_cmd_list(right_part, "Partie droite après parenthèse+opérateur");

				node->right = build_ast(right_part, minishell);
			}

			return (node);
		}
	}

	// 4. PRIORITÉ #4: Traiter les pipes implicites
	if (has_implicit_pipe(cleaned_list))
	{
		printf("DEBUG: Traitement d'un groupe de pipes implicites\n");
		node = create_pipe_node(cleaned_list, minishell);
		if (node)
			return (node);
	}

	// 5. Cas de base : commande simple
	printf("DEBUG: Création d'un noeud commande simple: %s\n",
		cleaned_list && cleaned_list->command_raw ? cleaned_list->command_raw : "(null)");
	return (create_command_node(cleaned_list, minishell));
}

