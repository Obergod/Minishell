/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_logic.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 19:15:42 by ufalzone          #+#    #+#             */
/*   Updated: 2025/05/16 17:16:38 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/ast.h"

t_ast_node	*init_logic_node(t_ast_node *left, t_cmd **cmd_list,
		t_minishell *minishell, int *paren_count)
{
	enum e_node_type	op_type;
	t_cmd				*op_cmd;
	t_ast_node			*logic_node;

	if ((*cmd_list)->logic_operator_type == AND)
		op_type = NODE_AND;
	else
		op_type = NODE_OR;
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
