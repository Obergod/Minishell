/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 20:03:56 by mafioron          #+#    #+#             */
/*   Updated: 2025/04/05 20:03:56 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/main.h"

void	prefix_exec(t_ast_node *node, t_ast_node *head, t_minishell *minishell)
{
	if (!node)
		return ;
	process(node, head, minishell);
	if (node->left && node->left->type != NODE_CMD && node->left->type != NODE_PIPE)
		prefix_exec(node->left, head, minishell);
	if (node->right && node->right->type != NODE_CMD && node->left->type != NODE_PIPE)
		prefix_exec(node->right, head, minishell);
}


void	process(t_ast_node *node, t_ast_node *head, t_minishell *minishell)
{
	if (node->type == NODE_PIPE)
		exec_pipes(node, head, minishell);
	else if (node->type == NODE_CMD)
		exec_cmd(node, head, minishell);
}


