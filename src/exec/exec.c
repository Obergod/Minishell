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
	pid_t	pid;
	int		status;

	if (!node)
		return ;
	pid = 456;
	if (node->subshell == 1)
		pid = fork();
	else if (node == head && skip_cmd(node))
		return ;
	if (pid == 0 || node->subshell == 0)
	{
		if (is_cmd(node) && (!ft_strcmp(node->cmd->command[0], "cd") ||
				!ft_strcmp(node->cmd->command[0], "export")))
		{
			if (!ft_strcmp(node->cmd->command[0], "export"))
				ft_export(node->cmd->command, minishell);
			else
				minishell->exit_status = ft_cd(node->cmd->command, minishell);
			return ;
		}
		process(node, head, minishell);
	}
	if (pid == 0)
		exit(minishell->exit_status);
	if (node->subshell == 1)
	{
		waitpid(pid, &status, 0);
		minishell->exit_status = WEXITSTATUS(status);
	}
}


void	process(t_ast_node *node, t_ast_node *head, t_minishell *minishell)
{
	if (node->type == NODE_PIPE)
		minishell->exit_status = exec_pipes(node, head, minishell);
	else if (node->type == NODE_CMD)
	{
		node->cmd->command = expand_vars(node->cmd->command, minishell);
		minishell->exit_status = exec_cmd(node, head, minishell);
	}
	else if (node->type == NODE_AND || node->type == NODE_OR)
		exec_log_operators(node, head, minishell);
	if (minishell->exit_status == 126 || minishell->exit_status == 127)
		perror(node->cmd->command[0]);
}


