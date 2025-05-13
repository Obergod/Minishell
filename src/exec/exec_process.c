/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:36:18 by mafioron          #+#    #+#             */
/*   Updated: 2025/05/13 17:36:21 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/main.h"

void	handle_child_process(t_ast_node *node, t_ast_node *head,
				t_minishell *minishell, t_redir *redirections)
{
	t_ast_node	dummy;
	t_cmd		dummy_cmd;
	int			fd_in;
	int			fd_out;

	if (redirections)
	{
		ft_memset(&dummy, 0, sizeof(t_ast_node));
		ft_memset(&dummy_cmd, 0, sizeof(t_cmd));
		dummy.type = NODE_CMD;
		dummy.cmd = &dummy_cmd;
		dummy.cmd->redirs = redirections;
		fd_in = -1;
		fd_out = -1;
		if (handle_redir(&dummy, minishell, &fd_in, &fd_out) == 1)
			clean_exit(EXIT_FAILURE, minishell);
	}
	if (node->cmd && handle_builtins(node, minishell) == 1)
		clean_exit(minishell->exit_status, minishell);
	process(node, head, minishell);
	clean_exit(minishell->exit_status, minishell);
}

void	handle_subshell(t_ast_node *node, t_ast_node *head,
				t_minishell *minishell)
{
	pid_t	pid;
	int		status;
	t_redir	*redirections;

	redirections = find_redirections_in_ast(head);
	pid = fork();
	if (pid == 0)
		handle_child_process(node, head, minishell, redirections);
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		minishell->exit_status = WEXITSTATUS(status);
	}
	else
		perror("fork");
}

void	handle_cmd_process(t_ast_node *node, t_ast_node *head,
				t_minishell *minishell)
{
	t_fds	fds;

	if ((!node->cmd->command || node->cmd->command[0] == 0)
		&& node->cmd->is_redirect == 1)
	{
		if (handle_redir(node, minishell, &fds.fd_in, &fds.fd_out) == 1)
			clean_exit(EXIT_FAILURE, minishell);
		return ;
	}
	node->cmd->command = expand_vars(node->cmd->command, minishell);
	if (!node->cmd->command || node->cmd->command[0] == 0)
		return ;
	minishell->exit_status = exec_cmd(node, head, minishell);
}

void	handle_command_not_found(t_ast_node *node, t_minishell *minishell)
{
	if (minishell->exit_status == 127)
	{
		if (node->type == NODE_CMD && node->cmd->command
			&& node->cmd->command[0])
		{
			ft_putstr_fd(node->cmd->command[0], 2);
			ft_putendl_fd(": command not found", 2);
		}
	}
	else if (minishell->exit_status == 126)
		perror(node->cmd->command[0]);
}
