/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:01:36 by ufalzone          #+#    #+#             */
/*   Updated: 2025/05/20 13:36:28 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

t_cmd	*new_cmd(t_minishell *minishell)
{
	t_cmd	*new;
	int		i;

	new = gc_malloc(sizeof(t_cmd), minishell->gc);
	new->command_raw = NULL;
	new->command = gc_malloc(sizeof(char *) * 4, minishell->gc);
	new->cmd_path = NULL;
	i = -1;
	while (++i < 4)
		new->command[i] = NULL;
	new->_arg_capacity = 4;
	new->_arg_count = 0;
	new->logic_operator_type = NONE;
	new->is_redirect = 0;
	new->redirs = NULL;
	new->heredoc = NULL;
	new->heredoc_fd = -1;
	new->next = NULL;
	return (new);
}

char	**resize_command_array(t_cmd *cmd, t_gc_head *gc)
{
	char	**new_command;
	size_t	i;

	new_command = gc_malloc(sizeof(char *) * (cmd->_arg_capacity * 2), gc);
	if (!new_command)
		return (NULL);
	i = 0;
	while (i < cmd->_arg_count)
	{
		new_command[i] = cmd->command[i];
		i++;
	}
	while (i < cmd->_arg_capacity * 2)
	{
		new_command[i] = NULL;
		i++;
	}
	return (new_command);
}

void	add_arg_to_cmd(t_cmd *cmd, char *str, t_gc_head *gc)
{
	char	**new_command;

	if (cmd->command_raw == NULL)
		cmd->command_raw = gc_strdup(str, gc);
	else
		cmd->command_raw = gc_strjoin_three(cmd->command_raw, " ", str, gc);
	if (cmd->_arg_count >= cmd->_arg_capacity - 1)
	{
		new_command = resize_command_array(cmd, gc);
		if (new_command)
		{
			cmd->command = new_command;
			cmd->_arg_capacity = cmd->_arg_capacity * 2;
		}
	}
	cmd->command[cmd->_arg_count] = gc_strdup(str, gc);
	cmd->command[cmd->_arg_count + 1] = NULL;
	cmd->_arg_count++;
}

void	add_cmd_to_list(t_cmd **cmd_list, t_cmd *current)
{
	t_cmd	*tmp;

	if (*cmd_list == NULL)
	{
		*cmd_list = current;
		return ;
	}
	tmp = *cmd_list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = current;
}
