/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 15:20:50 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/25 19:13:31 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/ast.h"

t_ast_node *init_ast_node(enum node_type type, t_cmd *cmd, t_minishell *minishell)
{
	t_ast_node *node;

	node = gc_malloc(sizeof(t_ast_node), minishell->gc);
	if (!node)
		return (NULL);

	node->type = type;
	node->cmd = cmd;
	node->subshell = 0;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_ast_node *build_ast_recursive(t_cmd **cmd_list, t_minishell *minishell,
								int *parenthesis_count);

static int handle_parenthesis(t_cmd **cmd_list, t_ast_node **ast,
							t_minishell *minishell, int *parenthesis_count)
{
	t_ast_node *subshell_node;
	t_cmd *tmp;
	int op_type;

	if ((*cmd_list)->logic_operator_type == OPEN_PARENTHESIS)
	{
		(*parenthesis_count)++;
		tmp = *cmd_list;
		*cmd_list = (*cmd_list)->next;
		if (!*cmd_list)
			return (0);

		subshell_node = build_ast_recursive(cmd_list, minishell, parenthesis_count);
		if (!subshell_node)
			return (0);

		subshell_node->subshell = 1;
		*ast = subshell_node;

		// Si après la parenthèse il y a un opérateur, le traiter
		if (*cmd_list && ((*cmd_list)->logic_operator_type == PIPE ||
						(*cmd_list)->logic_operator_type == AND ||
						(*cmd_list)->logic_operator_type == OR))
		{
			t_ast_node *op_node;

			if ((*cmd_list)->logic_operator_type == PIPE)
				op_type = NODE_PIPE;
			else if ((*cmd_list)->logic_operator_type == AND)
				op_type = NODE_AND;
			else
				op_type = NODE_OR;

			op_node = init_ast_node(op_type, *cmd_list, minishell);
			if (!op_node)
				return (0);

			op_node->left = *ast;

			// Avancer au token suivant
			*cmd_list = (*cmd_list)->next;
			if (!*cmd_list)
				return (0);

			// Construire la partie droite de l'opérateur
			op_node->right = build_ast_recursive(cmd_list, minishell, parenthesis_count);
			if (!op_node->right)
				return (0);

			*ast = op_node;
		}

		return (1);
	}
	else if ((*cmd_list)->logic_operator_type == CLOSE_PARENTHESIS)
	{
		(*parenthesis_count)--;
		if (*parenthesis_count < 0)
			return (0);

		*cmd_list = (*cmd_list)->next;
		return (2);
	}

	return (1);
}

static t_ast_node *build_expr(t_cmd **cmd_list, t_minishell *minishell,
							int *parenthesis_count)
{
	t_ast_node *node;
	int result;

	if (!cmd_list || !*cmd_list)
		return (NULL);

	if ((*cmd_list)->logic_operator_type == OPEN_PARENTHESIS ||
		(*cmd_list)->logic_operator_type == CLOSE_PARENTHESIS)
	{
		result = handle_parenthesis(cmd_list, &node, minishell, parenthesis_count);
		if (result == 0)
			return (NULL);
		else if (result == 2)
			return (NULL);
		return (node);
	}

	// Si le token est un opérateur logique sans commande, on le saute
	if ((*cmd_list)->logic_operator_type != 0 && !(*cmd_list)->command_raw)
	{
		*cmd_list = (*cmd_list)->next;
		return (build_expr(cmd_list, minishell, parenthesis_count));
	}

	// Création du nœud de commande
	node = init_ast_node(NODE_CMD, *cmd_list, minishell);
	if (!node)
		return (NULL);

	*cmd_list = (*cmd_list)->next;

	return (node);
}

// Gestion des pipes (priorité plus élevée que AND/OR)
static t_ast_node *handle_pipes(t_cmd **cmd_list, t_minishell *minishell, int *parenthesis_count)
{
	t_ast_node *left;
	t_ast_node *root;
	t_cmd *op_cmd;

	// Construction du nœud de gauche
	left = build_expr(cmd_list, minishell, parenthesis_count);
	if (!left)
		return (NULL);

	// Si on trouve un pipe, on le traite en priorité
	while (*cmd_list && (*cmd_list)->logic_operator_type == PIPE)
	{
		op_cmd = *cmd_list;
		*cmd_list = (*cmd_list)->next;
		if (!*cmd_list)
			return (NULL);

		root = init_ast_node(NODE_PIPE, op_cmd, minishell);
		if (!root)
			return (NULL);

		root->left = left;

		// Construction du nœud de droite (récursivement pour gérer des pipes en chaîne)
		root->right = build_expr(cmd_list, minishell, parenthesis_count);
		if (!root->right)
			return (NULL);

		// Mise à jour du nœud gauche pour le prochain pipe éventuel
		left = root;
	}

	return (left);
}

t_ast_node *build_ast_recursive(t_cmd **cmd_list, t_minishell *minishell,
								int *parenthesis_count)
{
	t_ast_node *left;
	t_ast_node *root;
	enum node_type op_type;
	t_cmd *op_cmd;

	if (!*cmd_list)
		return (NULL);

	// Gestion des parenthèses fermantes au début
	if ((*cmd_list)->logic_operator_type == CLOSE_PARENTHESIS)
	{
		(*parenthesis_count)--;
		if (*parenthesis_count < 0)
			return (NULL);
		*cmd_list = (*cmd_list)->next;
		return (NULL);
	}

	// Construction du nœud de gauche en gérant les pipes (priorité supérieure)
	left = handle_pipes(cmd_list, minishell, parenthesis_count);
	if (!left)
		return (NULL);

	// Fin de la liste ou parenthèse fermante
	if (!*cmd_list)
		return (left);

	// Gestion des parenthèses fermantes
	if ((*cmd_list)->logic_operator_type == CLOSE_PARENTHESIS)
	{
		(*parenthesis_count)--;
		if (*parenthesis_count < 0)
			return (NULL);
		left->subshell_redir = (*cmd_list)->redirs;
		*cmd_list = (*cmd_list)->next;
		return (left);
	}

	// Gestion des opérateurs logiques AND/OR (priorité inférieure aux pipes)
	if ((*cmd_list)->logic_operator_type == AND ||
		(*cmd_list)->logic_operator_type == OR)
	{
		if ((*cmd_list)->logic_operator_type == AND)
			op_type = NODE_AND;
		else
			op_type = NODE_OR;

		op_cmd = *cmd_list;

		*cmd_list = (*cmd_list)->next;
		if (!*cmd_list)
			return (NULL);

		root = init_ast_node(op_type, op_cmd, minishell);
		if (!root)
			return (NULL);

		root->left = left;
		root->right = build_ast_recursive(cmd_list, minishell, parenthesis_count);
		if (!root->right)
			return (NULL);

		return (root);
	}

	return (left);
}

// Compte le nombre de parenthèses ouvertes et fermées pour vérifier l'équilibre
static int check_parenthesis_balance(t_cmd *cmd_list)
{
	int count;
	t_cmd *current;

	count = 0;
	current = cmd_list;
	while (current)
	{
		if (current->logic_operator_type == OPEN_PARENTHESIS)
			count++;
		else if (current->logic_operator_type == CLOSE_PARENTHESIS)
			count--;

		if (count < 0)
			return (0); // Parenthèse fermante sans ouvrante

		current = current->next;
	}

	return (count == 0); // Toutes les parenthèses ouvertes sont fermées
}

t_ast_node *build_ast(t_cmd *cmd_list, t_minishell *minishell)
{
	int parenthesis_count;
	t_ast_node *root;

	if (!cmd_list)
		return (NULL);

	if (cmd_list->logic_operator_type == OPEN_PARENTHESIS && !cmd_list->command_raw && !cmd_list->next)
		return (NULL);

	// Vérifier l'équilibre des parenthèses avant de construire l'AST
	if (!check_parenthesis_balance(cmd_list))
		return (NULL);

	parenthesis_count = 0;
	root = build_ast_recursive(&cmd_list, minishell, &parenthesis_count);

	if (parenthesis_count != 0)
		return (NULL);

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
