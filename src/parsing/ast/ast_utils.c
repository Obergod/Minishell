/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:30:00 by ufalzone          #+#    #+#             */
/*   Updated: 2025/05/13 12:30:00 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/ast.h"

t_ast_node *init_ast_node(enum e_node_type type, t_cmd *cmd, t_minishell *minishell)
{
	t_ast_node *node;

	node = gc_malloc(sizeof(t_ast_node), minishell->gc);
	if (!node)
		return (NULL);
	node->type = type;
	node->cmd = cmd;
	node->left = NULL;
	node->right = NULL;
	node->subshell = 0;
	node->subshell_redir = NULL;
	return (node);
}

int check_parenthesis_balance(t_cmd *cmd_list)
{
	int count;
	t_cmd *cur;

	count = 0;
	cur = cmd_list;
	while (cur)
	{
		if (cur->logic_operator_type == OPEN_PARENTHESIS)
			count++;
		else if (cur->logic_operator_type == CLOSE_PARENTHESIS)
			count--;
		if (count < 0)
			return (0);
		cur = cur->next;
	}
	return (count == 0);
} 