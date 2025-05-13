/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 15:20:50 by ufalzone          #+#    #+#             */
/*   Updated: 2025/05/13 13:33:51 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/ast.h"

// Prototypes des utilitaires
int check_parenthesis_balance(t_cmd *cmd_list);
t_ast_node *init_ast_node(enum node_type type, t_cmd *cmd, t_minishell *minishell);

// Prototype pour éviter l'implicite
static t_ast_node *build_ast_recursive(t_cmd **cmd_list, t_minishell *minishell, int *paren_count);

// Gère la création d'un noeud d'expression (commande ou sous-shell)
static t_ast_node *build_expr_parenthesis(t_cmd **cmd_list, t_minishell *minishell, int *paren_count)
{
	t_ast_node *subshell;

	(*paren_count)++;
	*cmd_list = (*cmd_list)->next;
	subshell = build_ast_recursive(cmd_list, minishell, paren_count);
	if (!subshell)
		return (NULL);
	subshell->subshell = 1;
	if (*cmd_list && (*cmd_list)->redirs)
		subshell->subshell_redir = (*cmd_list)->redirs;
	if (*cmd_list && (*cmd_list)->logic_operator_type == CLOSE_PARENTHESIS)
	{
		(*paren_count)--;
		*cmd_list = (*cmd_list)->next;
	}
	return (subshell);
}

static t_ast_node *build_expr_cmd(t_cmd **cmd_list, t_minishell *minishell)
{
	t_ast_node *node;

	node = init_ast_node(NODE_CMD, *cmd_list, minishell);
	if (!node)
		return (NULL);
	*cmd_list = (*cmd_list)->next;
	return (node);
}

static t_ast_node *build_expr(t_cmd **cmd_list, t_minishell *minishell, int *paren_count)
{
	t_ast_node *node;

	node = NULL;
	while (cmd_list && *cmd_list)
	{
		if ((*cmd_list)->logic_operator_type == OPEN_PARENTHESIS)
			return (build_expr_parenthesis(cmd_list, minishell, paren_count));
		if ((*cmd_list)->logic_operator_type == CLOSE_PARENTHESIS)
		{
			(*paren_count)--;
			*cmd_list = (*cmd_list)->next;
			return (NULL);
		}
		if ((*cmd_list)->logic_operator_type != 0 && !(*cmd_list)->command_raw)
		{
			*cmd_list = (*cmd_list)->next;
			continue;
		}
		return (build_expr_cmd(cmd_list, minishell));
	}
	return (NULL);
}

static t_ast_node *init_pipe_node(t_ast_node *left, t_cmd **cmd_list, t_minishell *minishell, int *paren_count)
{
	t_cmd *op_cmd;
	t_ast_node *pipe_node;

	op_cmd = *cmd_list;
	*cmd_list = (*cmd_list)->next;
	pipe_node = init_ast_node(NODE_PIPE, op_cmd, minishell);
	if (!pipe_node)
		return (NULL);
	pipe_node->left = left;
	pipe_node->right = build_expr(cmd_list, minishell, paren_count);
	if (!pipe_node->right)
		return (NULL);
	return (pipe_node);
}

static t_ast_node *handle_pipes(t_cmd **cmd_list, t_minishell *minishell, int *paren_count)
{
	t_ast_node *left;

	left = build_expr(cmd_list, minishell, paren_count);
	while (left && *cmd_list && (*cmd_list)->logic_operator_type == PIPE)
	{
		left = init_pipe_node(left, cmd_list, minishell, paren_count);
	}
	return (left);
}

static t_ast_node *init_logic_node(t_ast_node *left, t_cmd **cmd_list, t_minishell *minishell, int *paren_count)
{
	enum node_type op_type;
	t_cmd *op_cmd;
	t_ast_node *logic_node;

	op_type = (*cmd_list)->logic_operator_type == AND ? NODE_AND : NODE_OR;
	op_cmd = *cmd_list;
	*cmd_list = (*cmd_list)->next;
	logic_node = init_ast_node(op_type, op_cmd, minishell);
	if (!logic_node)
		return (NULL);
	logic_node->left = left;
	logic_node->right = build_ast_recursive(cmd_list, minishell, paren_count);
	if (!logic_node->right)
		return (NULL);
	return (logic_node);
}

static t_ast_node *build_ast_recursive(t_cmd **cmd_list, t_minishell *minishell, int *paren_count)
{
	t_ast_node *left;

	if (!*cmd_list)
		return (NULL);
	if ((*cmd_list)->logic_operator_type == CLOSE_PARENTHESIS)
	{
		(*paren_count)--;
		*cmd_list = (*cmd_list)->next;
		return (NULL);
	}
	left = handle_pipes(cmd_list, minishell, paren_count);
	if (!left)
		return (NULL);
	while (*cmd_list)
	{
		if ((*cmd_list)->logic_operator_type == CLOSE_PARENTHESIS)
		{
			(*paren_count)--;
			*cmd_list = (*cmd_list)->next;
			return (left);
		}
		if ((*cmd_list)->logic_operator_type == AND || (*cmd_list)->logic_operator_type == OR)
			return (init_logic_node(left, cmd_list, minishell, paren_count));
		break;
	}
	return (left);
}

static int is_invalid_cmd_list(t_cmd *cmd_list)
{
	if (!cmd_list)
		return (1);
	if (cmd_list->logic_operator_type == OPEN_PARENTHESIS && !cmd_list->command_raw && !cmd_list->next)
		return (1);
	if (!check_parenthesis_balance(cmd_list))
		return (1);
	return (0);
}

t_ast_node *build_ast(t_cmd *cmd_list, t_minishell *minishell)
{
	int paren_count;
	t_cmd *current;
	t_ast_node *root;

	if (is_invalid_cmd_list(cmd_list))
		return (NULL);
	paren_count = 0;
	current = cmd_list;
	root = build_ast_recursive(&current, minishell, &paren_count);
	if (paren_count != 0)
		return (NULL);
	return (root);
}
