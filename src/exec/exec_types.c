/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_types.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:50:50 by mafioron          #+#    #+#             */
/*   Updated: 2025/04/25 18:40:41 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/main.h"

void	handle_cmd(t_ast_node *node, t_minishell *minishell,
				int *fd_in, int *fd_out)
{
	if (handle_redir(node, minishell, fd_in, fd_out) == 1)
		clean_exit(EXIT_FAILURE, minishell);
	if (is_fork_builtin(node))
	{
		minishell->exit_status = exec_fork_builtins(node, minishell);
		clean_exit(minishell->exit_status, minishell);
	}
	execve(node->cmd->cmd_path, node->cmd->command, minishell->env_array);
	perror("exec failed");
	clean_exit(EXIT_FAILURE, minishell);
}

int	handle_path(t_minishell *minishell, t_ast_node *node, t_wait *p_s)
{
	node->cmd->cmd_path = get_cmd_path(minishell,
			node->cmd->command[0], &p_s->status);
	if (p_s->status)
		return (p_s->status);
	if (!node->cmd->cmd_path)
		return (1);
	if (access(node->cmd->cmd_path, X_OK == -1))
		return (126);
	return (0);
}

int	exec_cmd(t_ast_node *node, t_ast_node *head, t_minishell *minishell)
{
	t_wait	p_s;
	t_fds	fds;

	fds.fd_in = -1;
	fds.fd_out = -1;
	p_s.status = 0;
	if (!is_fork_builtin(node))
	{
		p_s.status = handle_path(minishell, node, &p_s);
		if (p_s.status)
			return (p_s.status);
	}
	if (node == head)
	{
		p_s.pid = fork();
		if (p_s.pid == 0)
		{
			reset_signals_child();
			handle_cmd(node, minishell, &fds.fd_in, &fds.fd_out);
		}
		else if (p_s.pid < 0)
			return (perror("fork failed"), 1);
		close_fds(&fds.fd_in, &fds.fd_out);
		return (wait_and_signal(p_s.pid, p_s.status, minishell));
	}
	else
		handle_cmd(node, minishell, &fds.fd_in, &fds.fd_out);
	return (0);
}

int	exec_log_operators(t_ast_node *node,
		t_ast_node *head, t_minishell *minishell)
{
	head = node->left;
	prefix_exec(node->left, head, minishell);
	if (node->type == NODE_AND && minishell->exit_status == 0)
	{
		head = node->right;
		prefix_exec(node->right, head, minishell);
	}
	else if (node->type == NODE_OR && minishell->exit_status != 0)
	{
		head = node->right;
		prefix_exec(node->right, head, minishell);
	}
	return (0);
}
