/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 19:10:03 by mafioron          #+#    #+#             */
/*   Updated: 2025/05/20 18:50:03 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/main.h"

static int	handle_heredoc_eof(char *delimiter)
{
	ft_putstr_fd("minishell: warning: here-document delimited by end-of-file",
		2);
	ft_putstr_fd(" (wanted '", 2);
	ft_putstr_fd(delimiter, 2);
	ft_putendl_fd("')", 2);
	return (1);
}

int	here_doc(char *delimiter, t_minishell *minishell)
{
	int		pipes[2];
	char	*line;
	char	*trimmed_line;

	if (pipe(pipes) == -1)
		return (perror("minishell: pipe"), -1);
	while (1)
	{
		ft_putstr_fd("heredoc> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (!line)
		{
			if (handle_heredoc_eof(delimiter))
				break ;
		}
		gc_alloc(line, minishell->gc);
		trimmed_line = ft_strtrim(line, "\n");
		gc_alloc(trimmed_line, minishell->gc);
		if (ft_strcmp(trimmed_line, delimiter) == 0)
			break ;
		line = expand_str(line, minishell);
		write(pipes[1], line, ft_strlen(line));
	}
	close(pipes[1]);
	return (pipes[0]);
}

int	handle_heredoc(t_redir *redir, int *fd_in, t_minishell *minishell)
{
	int	new_fd;

	if (*fd_in != -1)
		close(*fd_in);
	new_fd = here_doc(redir->file_or_delimiter, minishell);
	if (new_fd == -1)
	{
		perror("heredoc");
		return (1);
	}
	*fd_in = new_fd;
	return (0);
}

void	empty_heredoc(char *delimiter, t_minishell *minishell)
{
	char	*line;

	while (1)
	{
		line = readline("heredoc> ");
		gc_alloc(line, minishell->gc);
		if (!line)
		{
			ft_putstr_fd("warning: here-document at", 2);
			ft_putendl_fd(" line 1 delimited by end-of-file", 2);
			break ;
		}
		line = expand_str(line, minishell);
		if (ft_strcmp(line, delimiter) == 0)
			break ;
	}
}
