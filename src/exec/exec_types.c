/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_types.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:50:50 by mafioron          #+#    #+#             */
/*   Updated: 2025/04/07 18:50:51 by mafioron         ###   ########.fr       */
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
	return(WEXITSTATUS(pipes.exit_status_r));
}

int	exec_cmd(t_ast_node *node, t_ast_node *head, t_minishell *minishell)
{
	pid_t	pid;
	int		status;
	int	fd_in;
	int	fd_out;

	status = 0;
	node->cmd->cmd_path = get_cmd_path(minishell, node->cmd->command[0], &status);
	if (status)
		return (status);
	if (!node->cmd->cmd_path)
		return (1);
	if (access(node->cmd->cmd_path, X_OK == -1))
		return (126);
	if (node == head)
	{
		pid = fork();
		if (pid == 0)
		{
			if (handle_redir(node, minishell, &fd_in, &fd_out) == 1)
				exit(EXIT_FAILURE);
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
		return(WEXITSTATUS(status));
	}
	else
	{
		if (handle_redir(node, minishell, &fd_in, &fd_out) == 1)
			exit(EXIT_FAILURE);
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

