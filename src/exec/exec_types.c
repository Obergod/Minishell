/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_types.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:50:50 by mafioron          #+#    #+#             */
/*   Updated: 2025/04/20 17:47:44 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/main.h"

int	exec_pipes(t_ast_node *node, t_ast_node *head, t_minishell *minishell)
{
	t_pipe	pipes;

	if (pipe(pipes.pipes) == -1)
		return (1);
	pipes.pid_l = fork();
	if (pipes.pid_l < 0)
	{
		perror("Fork failed");
		return (1);
	}
	else if (pipes.pid_l == 0)
	{
		reset_signals_child();
		close(pipes.pipes[0]);
		dup2(pipes.pipes[1], STDOUT_FILENO);
		close(pipes.pipes[1]);
		prefix_exec(node->left, head, minishell);
		exit(EXIT_FAILURE);
	}
	pipes.pid_r = fork();
	if (pipes.pid_r < 0)
	{
		perror("Fork failed");
		return (1);
	}
	else if (pipes.pid_r == 0)
	{
		reset_signals_child();
		close(pipes.pipes[1]);
		dup2(pipes.pipes[0], STDIN_FILENO);
		close(pipes.pipes[0]);
		prefix_exec(node->right, head, minishell);
		exit(EXIT_FAILURE);
	}
	close(pipes.pipes[0]);
	close(pipes.pipes[1]);
	waitpid(pipes.pid_l, &pipes.exit_status_l, 0);
	waitpid(pipes.pid_r, &pipes.exit_status_r, 0);
	if (WIFSIGNALED(pipes.exit_status_l) || WIFSIGNALED(pipes.exit_status_r))
	{
		if (WTERMSIG(pipes.exit_status_l) == SIGQUIT || WTERMSIG(pipes.exit_status_r) == SIGQUIT)
		{
			minishell->exit_status = 131;
			ft_putendl_fd("Quit (core dumped)", 2);
		}
		else if (WTERMSIG(pipes.exit_status_l) == SIGINT || WTERMSIG(pipes.exit_status_r) == SIGINT  || g_sig_received == 130)
        	minishell->exit_status = 130;
		return (minishell->exit_status);
	}
	return(WEXITSTATUS(pipes.exit_status_r));
}

int	exec_cmd(t_ast_node *node, t_ast_node *head, t_minishell *minishell)
{
	pid_t	pid;
	int		status;
	int	fd_in;
	int	fd_out;

	status = 0;
	if (!is_fork_builtin(node))
	{
		node->cmd->cmd_path = get_cmd_path(minishell, node->cmd->command[0], &status);
		if (status)
			return (status);
		if (!node->cmd->cmd_path)
			return (1);
		if (access(node->cmd->cmd_path, X_OK == -1))
			return (126);
		}
	if (node == head)
	{
		pid = fork();
		if (pid == 0)
		{
			reset_signals_child();
			if (handle_redir(node, minishell, &fd_in, &fd_out) == 1)
				exit(EXIT_FAILURE);
			if (is_fork_builtin(node))
			{
				minishell->exit_status = exec_fork_builtins(node, minishell);
				exit(minishell->exit_status);
			}
			execve(node->cmd->cmd_path, node->cmd->command, minishell->env_array);
			perror("exec failed");
			exit(EXIT_FAILURE);
		}
		else if (pid < 0)
		{
			perror("fork failed");
			return (1);
		}
		waitpid(pid, &status, 0);
		if (g_sig_received == SIGINT)
		{
			minishell->exit_status = 130;
			g_sig_received = 0;
			return (minishell->exit_status);
		}
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGQUIT)
			{
				minishell->exit_status = 131;
				ft_putendl_fd("Quit (core dumped)", 2);
			}
			return (minishell->exit_status);
		}
		return(WEXITSTATUS(status));
	}
	else
	{
		if (handle_redir(node, minishell, &fd_in, &fd_out) == 1)
			exit(EXIT_FAILURE);
		if (is_fork_builtin(node))
		{
			minishell->exit_status = exec_fork_builtins(node, minishell);
			exit(minishell->exit_status);
		}
		execve(node->cmd->cmd_path, node->cmd->command, minishell->env_array);
		perror("exec failed");
		exit(EXIT_FAILURE);
	}
	if (fd_in)
		close(fd_in);
	if (fd_out)
		close(fd_out);
	return (0);
}

int	exec_log_operators(t_ast_node *node, t_ast_node *head, t_minishell *minishell)
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
