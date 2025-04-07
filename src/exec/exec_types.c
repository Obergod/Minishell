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
		return (-1);
	pipes.pid_l = fork();
	if (pipes.pid_l < 0)
	{
		perror("Fork failed");
		return (-1);
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
		return (-1);
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

	node->cmd->cmd_path = get_cmd_path(minishell, node->cmd->command[0]);
	if (!node->cmd->cmd_path)
		return (-1);
	if (node == head)
	{
		pid = fork();
		if (pid == 0)
		{
			handle_redir(node, minishell, &fd_in, &fd_out);
			execve(node->cmd->cmd_path, node->cmd->command, NULL);
			perror("exec failed");
			exit(EXIT_FAILURE);
		}
		else if (pid < 0)
		{
			perror("fork failed");
			return (-1);
		}
		waitpid(pid, &status, 0);
		return(WEXITSTATUS(status));
	}
	else
	{
		handle_redir(node, minishell, &fd_in, &fd_out);
		execve(node->cmd->cmd_path, node->cmd->command, NULL);
		perror("exec failed");
		exit(EXIT_FAILURE);
	}
	if (fd_in)
		close(fd_in);
	if (fd_out)
		close(fd_out);
	return (1);
}

static void	handle_line(char *line, char *limiter, int *pipes)
{
	if (!line)
	{
		free(limiter);
		close(pipes[1]);
		return ;
	}
	if (ft_strcmp(line, limiter))
		write(pipes[1], line, ft_strlen(line));
}

int	here_doc(char *limiter)
{
	char	*line;
	int		pipes[2];

	limiter = ft_strjoin(limiter, "\n");
	if (!limiter)
		exit(EXIT_FAILURE);
	if (pipe(pipes) == -1)
	{
		free(limiter);
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		write(1, "heredoc> ", 9);
		line = get_next_line(STDIN_FILENO);
		handle_line(line, limiter, pipes);
		if (!line || !ft_strcmp(line, limiter))
			break ;
		free(line);
	}
	close(pipes[1]);
	return (pipes[0]);
}

int	handle_redir(t_ast_node *node, t_minishell *minishell, int *fd_in, int *fd_out)
{

	if (!minishell)
		return (-1);
	while (node->cmd->redirs)
	{
		if (node->cmd->redirs->type == REDIR_IN)
		{
			*fd_in = open(node->cmd->redirs->file_or_delimiter, O_RDONLY);
			if (*fd_in == -1)
				perror(node->cmd->redirs->file_or_delimiter);
			dup2(*fd_in, STDIN_FILENO);
		}
		if (node->cmd->redirs->type == REDIR_HEREDOC)
		{
			*fd_in = here_doc(node->cmd->redirs->file_or_delimiter);
			dup2(*fd_in, STDIN_FILENO);
		}
		if (node->cmd->redirs->type == REDIR_OUT || node->cmd->redirs->type == REDIR_APPEND)
		{
			if (node->cmd->redirs->type == REDIR_APPEND)
				*fd_out = open(node->cmd->redirs->file_or_delimiter, O_WRONLY | O_CREAT | O_APPEND, 0644);
			else if (node->cmd->redirs->type == REDIR_OUT)
				*fd_out = open(node->cmd->redirs->file_or_delimiter, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (*fd_out == -1)
			{
				if (*fd_in)
					close(*fd_in);
				perror(node->cmd->redirs->file_or_delimiter);
				return (-1);
			}
			dup2(*fd_out, STDOUT_FILENO);
		}
		node->cmd->redirs = node->cmd->redirs->next;
	}
	return (0);
}
