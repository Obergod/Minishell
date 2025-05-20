/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_build.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 19:15:39 by ufalzone          #+#    #+#             */
/*   Updated: 2025/05/20 17:01:41 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/ast.h"

t_ast_node	*build_expr_parenthesis(t_cmd **cmd_list, t_minishell *minishell,
		int *paren_count)
{
	t_ast_node	*subshell;

	(*paren_count)++;
	*cmd_list = (*cmd_list)->next;
	subshell = build_ast_recursive(cmd_list, minishell, paren_count);
	if (!subshell)
		return (NULL);
	subshell->subshell = 1;
	process_closing_paren(&subshell, cmd_list, paren_count);
	return (subshell);
}

t_ast_node	*build_expr_cmd(t_cmd **cmd_list, t_minishell *minishell)
{
	t_ast_node	*node;

	node = init_ast_node(NODE_CMD, *cmd_list, minishell);
	if (!node)
		return (NULL);
	*cmd_list = (*cmd_list)->next;
	return (node);
}

t_ast_node	*build_expr(t_cmd **cmd_list, t_minishell *minishell,
		int *paren_count)
{
	t_ast_node	*node;

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
			continue ;
		}
		return (build_expr_cmd(cmd_list, minishell));
	}
	return (NULL);
}
