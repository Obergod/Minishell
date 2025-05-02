/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/04/27 19:10:06 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

// New recursive function to find redirections in the AST
t_redir *find_redirections_in_ast(t_ast_node *node)
{
	if (!node)
		return (NULL);
	
	if (node->subshell_redir)
		return (node->subshell_redir);
	
	t_redir *left_redirs = find_redirections_in_ast(node->left);
	if (left_redirs)
		return (left_redirs);
	
	return (find_redirections_in_ast(node->right));
}

void	prefix_exec(t_ast_node *node, t_ast_node *head, t_minishell *minishell)
{
	pid_t	pid;
	int		status;
	t_redir *redirections = NULL;

	if (!node)
		return ;
	if (node->subshell == 1)
	{
		redirections = find_redirections_in_ast(head);
		
		pid = fork();
		if (pid == 0)
		{
			if (redirections)
			{
				t_ast_node dummy;
				t_cmd dummy_cmd;
				
				ft_memset(&dummy, 0, sizeof(t_ast_node));
				ft_memset(&dummy_cmd, 0, sizeof(t_cmd));
				
				dummy.type = NODE_CMD;
				dummy.cmd = &dummy_cmd;
				dummy.cmd->redirs = redirections;
				
				int fd_in = -1;
				int fd_out = -1;
				if (handle_redir(&dummy, minishell, &fd_in, &fd_out) == 1)
					clean_exit(EXIT_FAILURE, minishell);
			}
			
			if (node->cmd && handle_builtins(node, minishell) == 1)
				clean_exit(minishell->exit_status, minishell);
			
			process(node, head, minishell);
			clean_exit(minishell->exit_status, minishell);
		}
		else if (pid > 0)
		{
			waitpid(pid, &status, 0);
			minishell->exit_status = WEXITSTATUS(status);
		}
		else
		{
			perror("fork");
			return;
		}
	}
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
	t_fds	fds;
	exec_setup_signals();
	
	fds.fd_in == -1;
	fds.fd_out == -1;
	if (node->type == NODE_PIPE)
		minishell->exit_status = exec_pipes(node, head, minishell);
	else if (node->type == NODE_CMD)
	{
		if ((!node->cmd->command || node->cmd->command[0] == 0) && node->cmd->is_redirect == 1)
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
	else if (node->type == NODE_AND || node->type == NODE_OR)
		exec_log_operators(node, head, minishell);

	if (minishell->exit_status == 127)
	{
		if (node->type == NODE_CMD && node->cmd->command && node->cmd->command[0])
		{
			ft_putstr_fd(node->cmd->command[0], 2);
			ft_putendl_fd(": command not found", 2);
		}
	}
	else if (minishell->exit_status == 126)
		perror(node->cmd->command[0]);
	interactive_setup_signals();
	interactive_setup_signals();
}
