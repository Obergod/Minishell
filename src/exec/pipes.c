/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:35:46 by mafioron          #+#    #+#             */
/*   Updated: 2025/05/13 17:35:52 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/main.h"

int	handle_l_pipe(t_pipe pipes, t_ast_node *node, t_ast_node *head,
		t_minishell *minishell)
{
	if (pipes.pid_l < 0)
	{
		perror("Fork failed");
		return (1);
	}
	else if (pipes.pid_l == 0)
	{
		exec_setup_signals();
		exec_setup_signals();
		close(pipes.pipes[0]);
		if (dup2(pipes.pipes[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 failed");
			clean_exit(minishell->exit_status, minishell);
		}
		close(pipes.pipes[1]);
		prefix_exec(node->left, head, minishell);
		interactive_setup_signals();
		interactive_setup_signals();
		clean_exit(EXIT_FAILURE, minishell);
		clean_exit(minishell->exit_status, minishell);
	}
	return (0);
}

int	handle_r_pipe(t_pipe pipes, t_ast_node *node, t_ast_node *head,
		t_minishell *minishell)
{
	if (pipes.pid_r < 0)
	{
		perror("Fork failed");
		return (1);
	}
	else if (pipes.pid_r == 0)
	{
		exec_setup_signals();
		exec_setup_signals();
		close(pipes.pipes[1]);
		if (dup2(pipes.pipes[0], STDIN_FILENO) == -1)
		{
			perror("dup2 failed");
			clean_exit(minishell->exit_status, minishell);
		}
		close(pipes.pipes[0]);
		prefix_exec(node->right, head, minishell);
		interactive_setup_signals();
		interactive_setup_signals();
		clean_exit(EXIT_FAILURE, minishell);
		clean_exit(minishell->exit_status, minishell);
	}
	return (0);
}

int	wait_pipe_children(t_pipe *pipes, t_minishell *minishell)
{
	waitpid(pipes->pid_l, &pipes->exit_status_l, 0);
	waitpid(pipes->pid_r, &pipes->exit_status_r, 0);
	if (WIFSIGNALED(pipes->exit_status_l) || WIFSIGNALED(pipes->exit_status_r))
	{
		if (WTERMSIG(pipes->exit_status_l) == SIGQUIT
			|| WTERMSIG(pipes->exit_status_r) == SIGQUIT)
		{
			minishell->exit_status = 131;
			ft_putendl_fd("Quit (core dumped)", 2);
			return (minishell->exit_status);
		}
		if (WTERMSIG(pipes->exit_status_l) == SIGINT
			|| WTERMSIG(pipes->exit_status_r) == SIGINT)
		{
			minishell->exit_status = 130;
			return (minishell->exit_status);
		}
	}
	return (WEXITSTATUS(pipes->exit_status_r));
}

int	exec_pipes(t_ast_node *node, t_ast_node *head, t_minishell *minishell)
{
	t_pipe	pipes;
	t_fds	fds;

	fds.fd_in = -1;
	fds.fd_out = -1;
	if (pipe(pipes.pipes) == -1)
		return (1);
	pipes.pid_l = fork();
	if (handle_l_pipe(pipes, node, head, minishell))
		return (close_pipes(pipes.pipes), 1);
	pipes.pid_r = fork();
	if (handle_r_pipe(pipes, node, head, minishell))
		return (close_pipes(pipes.pipes), 1);
	close_pipes(pipes.pipes);
	close_fds(&fds.fd_in, &fds.fd_out);
	return (wait_pipe_children(&pipes, minishell));
}
