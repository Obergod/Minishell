/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:37:54 by ufalzone          #+#    #+#             */
/*   Updated: 2025/05/20 18:39:47 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/main.h"

static char	*read_heredoc_content(int fd, t_minishell *minishell)
{
	char	*line;
	char	*content;

	content = gc_strdup("", minishell->gc);
	line = get_next_line(fd);
	while (line)
	{
		gc_alloc(line, minishell->gc);
		content = gc_strjoin(content, line, minishell->gc);
		line = get_next_line(fd);
	}
	return (content);
}

void	ast_heredoc(t_ast_node *node, t_minishell *minishell)
{
	t_redir	*current;
	int		fd;

	if (!node)
		return ;
	ast_heredoc(node->left, minishell);
	if (node->type == NODE_CMD && node->cmd && node->cmd->redirs)
	{
		current = node->cmd->redirs;
		while (current)
		{
			if (current->type == REDIR_HEREDOC)
			{
				fd = here_doc(current->file_or_delimiter, minishell);
				node->cmd->heredoc = read_heredoc_content(fd, minishell);
				close(fd);
			}
			current = current->next;
		}
	}
	ast_heredoc(node->right, minishell);
}

int	process_heredoc_redir(t_ast_node *node, t_redir *cur, int *fd_in,
		t_minishell *minishell)
{
	int	pipes_hd[2];

	if (node->cmd->heredoc)
	{
		if (pipe(pipes_hd) == -1)
			return (perror("minishell: pipe"), 1);
		write(pipes_hd[1], node->cmd->heredoc, ft_strlen(node->cmd->heredoc));
		close(pipes_hd[1]);
		*fd_in = pipes_hd[0];
		return (0);
	}
	return (handle_heredoc(cur, fd_in, minishell));
}
