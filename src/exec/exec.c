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
	else if (node == head && skip_cmd(node))
		return ;
	else if (is_cmd(node) && !ft_strcmp(node->cmd->command[0], "cd"))
	{
		minishell->exit_status = ft_cd(node->cmd->command, minishell);
		return ;
	}
	process(node, head, minishell);
}


void	process(t_ast_node *node, t_ast_node *head, t_minishell *minishell)
{
	if (node->type == NODE_PIPE)
		minishell->exit_status = exec_pipes(node, head, minishell);
	else if (node->type == NODE_CMD)
		minishell->exit_status = exec_cmd(node, head, minishell);
	else if (node->type == NODE_AND || node->type == NODE_OR)
		exec_log_operators(node, head, minishell);
	if (minishell->exit_status == 126 || minishell->exit_status == 127)
		perror(node->cmd->command[0]);
}


