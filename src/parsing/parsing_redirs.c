/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 16:45:13 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/27 18:50:02 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

t_redir	*new_redir(enum e_redir_type type, char *file_or_delimiter,
		t_gc_head *gc)
{
	t_redir	*redir;

	redir = gc_malloc(sizeof(t_redir), gc);
	redir->type = type;
	redir->file_or_delimiter = gc_strdup(file_or_delimiter, gc);
	redir->next = NULL;
	return (redir);
}

void	add_redir_to_cmd(t_cmd *cmd, t_redir *redir)
{
	t_redir	*tmp;

	if (cmd->redirs == NULL)
	{
		cmd->redirs = redir;
		return ;
	}
	tmp = cmd->redirs;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = redir;
}

void	t_redir_parsing(t_token *token, t_cmd **current_cmd,
		t_minishell *minishell)
{
	t_redir	*redir;

	(*current_cmd)->is_redirect = 1;
	if (ft_strcmp(token->str, "<") == 0)
	{
		redir = new_redir(REDIR_IN, token->next->str, minishell->gc);
		add_redir_to_cmd(*current_cmd, redir);
	}
	else if (ft_strcmp(token->str, ">") == 0)
	{
		redir = new_redir(REDIR_OUT, token->next->str, minishell->gc);
		add_redir_to_cmd(*current_cmd, redir);
	}
	else if (ft_strcmp(token->str, ">>") == 0)
	{
		redir = new_redir(REDIR_APPEND, token->next->str, minishell->gc);
		add_redir_to_cmd(*current_cmd, redir);
	}
	else if (ft_strcmp(token->str, "<<") == 0)
	{
		redir = new_redir(REDIR_HEREDOC, token->next->str, minishell->gc);
		add_redir_to_cmd(*current_cmd, redir);
	}
}
