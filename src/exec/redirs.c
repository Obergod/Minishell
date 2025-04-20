/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 17:34:03 by mafioron          #+#    #+#             */
/*   Updated: 2025/04/09 17:34:04 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/main.h"

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
	if (remove_quotes_redirs(node->cmd->redirs, minishell) == -1)
		return (1);
	while (node->cmd->redirs)
	{
		if (node->cmd->redirs->type == REDIR_IN)
		{
			*fd_in = open(node->cmd->redirs->file_or_delimiter, O_RDONLY);
			if (*fd_in == -1)
				perror(node->cmd->redirs->file_or_delimiter);
			if (check_file_accesss(node->cmd->redirs->file_or_delimiter, 0) == 1)
				return (1);
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
				return (1);
			}
			if (check_file_accesss(node->cmd->redirs->file_or_delimiter, 1) == 1)
				return (1);
			dup2(*fd_out, STDOUT_FILENO);
		}
		node->cmd->redirs = node->cmd->redirs->next;
	}
	return (0);
}
