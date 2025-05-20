/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_fix.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:01:19 by ufalzone          #+#    #+#             */
/*   Updated: 2025/05/20 17:03:33 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/ast.h"

t_ast_node	*process_logic_node(t_ast_node *left, t_cmd **cmd_list,
		t_minishell *minishell, int *paren_count)
{
	t_cmd				*op_cmd;
	enum e_node_type	op_type;
	t_ast_node			*right;
	t_ast_node			*node;

	op_cmd = *cmd_list;
	if (op_cmd->logic_operator_type == AND)
		op_type = NODE_AND;
	else
		op_type = NODE_OR;
	*cmd_list = (*cmd_list)->next;
	right = handle_pipes(cmd_list, minishell, paren_count);
	if (!right)
		return (NULL);
	node = init_ast_node(op_type, op_cmd, minishell);
	if (!node)
		return (NULL);
	node->left = left;
	node->right = right;
	return (node);
}

void	process_closing_paren(t_ast_node **left, t_cmd **cmd_list,
		int *paren_count)
{
	if (*cmd_list && (*cmd_list)->logic_operator_type == CLOSE_PARENTHESIS)
	{
		if ((*cmd_list)->redirs)
			(*left)->subshell_redir = (*cmd_list)->redirs;
		(*paren_count)--;
		*cmd_list = (*cmd_list)->next;
	}
}

t_ast_node	*build_ast_recursive_loop(t_ast_node *left, t_cmd **cmd_list,
		t_minishell *minishell, int *paren_count)
{
	t_ast_node	*node;

	while (*cmd_list && ((*cmd_list)->logic_operator_type == AND
			|| (*cmd_list)->logic_operator_type == OR))
	{
		node = process_logic_node(left, cmd_list, minishell, paren_count);
		if (!node)
			return (NULL);
		left = node;
	}
	process_closing_paren(&left, cmd_list, paren_count);
	return (left);
}

t_ast_node	*build_ast_recursive(t_cmd **cmd_list, t_minishell *minishell,
		int *paren_count)
{
	t_ast_node	*left;

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
	return (build_ast_recursive_loop(left, cmd_list, minishell, paren_count));
}
