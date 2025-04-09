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

int	is_empty_cmd(t_ast_node *node)
{
	return (node->cmd && node->cmd->command &&
			node->cmd->command[0] && node->cmd->command[0][0] == '\0');
}

void	prefix_exec(t_ast_node *node, t_ast_node *head, t_minishell *minishell)
{
	if (!node)
		return ;
	if (node == head && is_empty_cmd(node))
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
		minishell->exit_status = exec_pipes(node, head, minishell);
	else if (node->type == NODE_CMD)
		minishell->exit_status = exec_cmd(node, head, minishell);
	if (minishell->exit_status == 126 || minishell->exit_status == 127)
		perror(node->cmd->command[0]);
}


