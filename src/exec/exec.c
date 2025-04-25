/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 20:03:56 by mafioron          #+#    #+#             */
/*   Updated: 2025/04/25 17:48:51 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/main.h"

static int	handle_builtins(t_ast_node *node, t_minishell *minishell)
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

void	prefix_exec(t_ast_node *node, t_ast_node *head, t_minishell *minishell)
{
	pid_t	pid;
	int		status;

	if (!node)
		return ;
	pid = 1;
	if (node->subshell == 1)
		pid = fork();
	else if (node == head && skip_cmd(node))
		return ;
	if (pid == 0 || node->subshell == 0)
	{
		if (handle_builtins(node, minishell) == 1)
			return ;
		process(node, head, minishell);
	}
	if (pid == 0)
		clean_exit(minishell->exit_status, minishell);
	if (node->subshell == 1)
	{
		waitpid(pid, &status, 0);
		minishell->exit_status = WEXITSTATUS(status);
	}
}

void	process(t_ast_node *node, t_ast_node *head, t_minishell *minishell)
{
	exec_setup_signals();
	if (node->type == NODE_PIPE)
		minishell->exit_status = exec_pipes(node, head, minishell);
	else if (node->type == NODE_CMD)
	{
		node->cmd->command = expand_vars(node->cmd->command, minishell);
		if (!node->cmd->command || node->cmd->command[0] == 0)
			return ;
		minishell->exit_status = exec_cmd(node, head, minishell);
	}
	else if (node->type == NODE_AND || node->type == NODE_OR)
		exec_log_operators(node, head, minishell);
	if (minishell->exit_status == 126 || minishell->exit_status == 127)
		perror(node->cmd->command[0]);
	interactive_setup_signals();
}
