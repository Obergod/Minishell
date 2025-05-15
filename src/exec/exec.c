/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:36:18 by mafioron          #+#    #+#             */
/*   Updated: 2025/05/15 17:56:17 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/main.h"

int	handle_builtins(t_ast_node *node, t_minishell *minishell)
{
	if (is_cmd(node) && is_builtin(node))
	{
		node->cmd->command = expand_vars(node->cmd->command, minishell);
		if (!node->cmd->command || node->cmd->command[0] == 0)
			return (1);
		minishell->exit_status = exec_builtins(node, minishell);
		return (1);
	}
	return (0);
}

t_redir	*find_redirections_in_ast(t_ast_node *node)
{
	t_redir	*left_redirs;

	if (!node)
		return (NULL);
	if (node->subshell_redir)
		return (node->subshell_redir);
	left_redirs = find_redirections_in_ast(node->left);
	if (left_redirs)
		return (left_redirs);
	return (find_redirections_in_ast(node->right));
}

void	prefix_exec(t_ast_node *node, t_ast_node *head, t_minishell *minishell)
{
	if (!node)
		return ;
	if (node->subshell == 1)
		handle_subshell(node, head, minishell);
	else if (node == head && skip_cmd(node) && node->cmd->is_redirect != 1)
		return ;
	else
	{
		if (handle_builtins(node, minishell) == 1)
			return ;
		process(node, head, minishell);
	}
}

void	process(t_ast_node *node, t_ast_node *head, t_minishell *minishell)
{
	exec_setup_signals();
	if (node->type == NODE_PIPE)
		minishell->exit_status = exec_pipes(node, head, minishell);
	else if (node->type == NODE_CMD)
		handle_cmd_process(node, head, minishell);
	else if (node->type == NODE_AND || node->type == NODE_OR)
		exec_log_operators(node, head, minishell);
	handle_command_not_found(node, minishell);
	interactive_setup_signals();
}
