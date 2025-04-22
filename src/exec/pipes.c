/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 14:46:03 by mafioron          #+#    #+#             */
/*   Updated: 2025/04/22 14:46:04 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/main.h"

int	handle_l_pipe(t_pipe pipes, t_ast_node *node, t_ast_node *head, t_minishell *minishell)
{
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
	return (0);
}

int	handle_r_pipe(t_pipe pipes, t_ast_node *node, t_ast_node *head, t_minishell *minishell)
{
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
	return (0);
}

int	exec_pipes(t_ast_node *node, t_ast_node *head, t_minishell *minishell)
{
	t_pipe	pipes;

	if (pipe(pipes.pipes) == -1)
		return (1);
	pipes.pid_l = fork();
	if (handle_l_pipe(pipes, node, head, minishell))
		return (1);
	pipes.pid_r = fork();
	if (handle_r_pipe(pipes, node, head, minishell))
		return (1);
	close_pipes(pipes.pipes);
	waitpid(pipes.pid_l, &pipes.exit_status_l, 0);
	return(wait_and_signal(pipes.pid_l, pipes.exit_status_r, minishell));
}
