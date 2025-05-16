/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 19:15:46 by ufalzone          #+#    #+#             */
/*   Updated: 2025/05/16 17:16:55 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/ast.h"

t_ast_node	*init_pipe_node(t_ast_node *left, t_cmd **cmd_list,
		t_minishell *minishell, int *paren_count)
{
	t_cmd		*op_cmd;
	t_ast_node	*pipe_node;

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

t_ast_node	*handle_pipes(t_cmd **cmd_list, t_minishell *minishell,
		int *paren_count)
{
	t_ast_node	*left;

	left = build_expr(cmd_list, minishell, paren_count);
	while (left && *cmd_list && (*cmd_list)->logic_operator_type == PIPE)
	{
		left = init_pipe_node(left, cmd_list, minishell, paren_count);
	}
	return (left);
}
