/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:01:47 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/27 19:06:14 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"
#include "../../includes/token.h"
#include <string.h>

//<< en position 0
// EOF sera en position 1
// cmd en position 2

//< en position 0
// infile en position 1
// cmd en position 2

// aucun operator en 0
// commande sera en 0

// status : 0 = debut
// status : 1 = fin

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
	new->next = NULL;
	return (new);
}

static char	**resize_command_array(t_cmd *cmd, t_gc_head *gc)
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

void	t_logic_parsing(t_token *token, t_cmd **current_cmd)
{
	if (ft_strcmp(token->str, "&&") == 0)
		(*current_cmd)->logic_operator_type = AND;
	else if (ft_strcmp(token->str, "||") == 0)
		(*current_cmd)->logic_operator_type = OR;
	else if (ft_strcmp(token->str, "(") == 0)
		(*current_cmd)->logic_operator_type = OPEN_PARENTHESIS;
	else if (ft_strcmp(token->str, ")") == 0)
		(*current_cmd)->logic_operator_type = CLOSE_PARENTHESIS;
	else if (ft_strcmp(token->str, "|") == 0)
		(*current_cmd)->logic_operator_type = PIPE;
}

static void	handle_parsing_operator(t_token *token, t_cmd **list,
		t_cmd **current, t_minishell *minishell)
{
	if ((*current)->command[0] || (*current)->redirs)
	{
		add_cmd_to_list(list, *current);
		*current = new_cmd(minishell);
	}
	t_logic_parsing(token, current);
	add_cmd_to_list(list, *current);
	*current = new_cmd(minishell);
}

t_cmd	*parsing(t_token *token, t_minishell *minishell)
{
	t_cmd	*current;
	t_cmd	*head;

	current = new_cmd(minishell);
	head = NULL;
	if (check_parsing(token) != SUCCESS)
		return (print_error(check_parsing(token)), NULL);
	while (token)
	{
		if (token->type == T_WORD)
			add_arg_to_cmd(current, token->str, minishell->gc);
		else if (token->type == T_REDIR)
		{
			t_redir_parsing(token, &current, minishell);
			token = token->next->next;
			continue ;
		}
		handle_parsing_operator(token, &head, &current, minishell);
		token = token->next;
	}
	if (current->command[0] || current->redirs)
		add_cmd_to_list(&head, current);
	return (head);
}
