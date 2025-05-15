/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 17:34:03 by mafioron          #+#    #+#             */
/*   Updated: 2025/05/15 18:11:44 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/main.h"

int	handle_input(t_redir *redir, int *fd_in)
{
	int	new_fd;

	if (*fd_in != -1)
		close(*fd_in);
	new_fd = open(redir->file_or_delimiter, O_RDONLY);
	if (new_fd == -1)
	{
		perror(redir->file_or_delimiter);
		return (1);
	}
	if (check_file_accesss(redir->file_or_delimiter, 0) == 1)
		return (1);
	*fd_in = new_fd;
	return (0);
}

int	handle_output(t_redir *redir, int *fd_out)
{
	int	new_fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (redir->type == REDIR_APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	if (*fd_out != -1)
		close(*fd_out);
	new_fd = open(redir->file_or_delimiter, flags, 0644);
	if (new_fd == -1)
	{
		perror(redir->file_or_delimiter);
		return (1);
	}
	if (check_file_accesss(redir->file_or_delimiter, 1) == 1)
		return (1);
	*fd_out = new_fd;
	return (0);
}

static int	apply_redirections(int *fd_in, int *fd_out)
{
	if (*fd_in != -1)
	{
		if (dup2(*fd_in, STDIN_FILENO) == -1)
			return (perror("minishell: dup2"), 1);
		close(*fd_in);
		*fd_in = -1;
	}
	if (*fd_out != -1)
	{
		if (dup2(*fd_out, STDOUT_FILENO) == -1)
			return (perror("minishell: dup2"), 1);
		close(*fd_out);
		*fd_out = -1;
	}
	return (0);
}

int	handle_redir(t_ast_node *node, t_minishell *minishell, int *fd_in,
		int *fd_out)
{
	t_redir	*cur;

	*fd_in = -1;
	*fd_out = -1;
	cur = node->cmd->redirs;
	if (remove_quotes_redirs(node->cmd->redirs, minishell) == -1)
		return (1);
	while (cur)
	{
		if (cur->type == REDIR_IN && handle_input(cur, fd_in))
			return (1);
		else if (cur->type == REDIR_HEREDOC && handle_heredoc(cur, fd_in,
				minishell))
			return (1);
		else if ((cur->type == REDIR_OUT || cur->type == REDIR_APPEND)
			&& handle_output(cur, fd_out))
			return (1);
		cur = cur->next;
	}
	return (apply_redirections(fd_in, fd_out));
}
