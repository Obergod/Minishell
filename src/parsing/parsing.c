/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:01:47 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/27 16:54:41 by ufalzone         ###   ########.fr       */
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
	new->redirs = NULL;
	new->next = NULL;
	return (new);
}

// creer command->size = 4
// si jai mit 3 arg dans command => arg_count = 3
void	add_arg_to_cmd(t_cmd *cmd, char *str, t_gc_head *gc)
{
	if (cmd->command_raw == NULL)
		cmd->command_raw = gc_strdup(str, gc);
	else
		cmd->command_raw = gc_strjoin_three(cmd->command_raw, " ", str, gc);
	if (cmd->_arg_count >= cmd->_arg_capacity - 1)
	{
		cmd->command = gc_strndup_array(cmd->command, cmd->_arg_capacity * 2,
				gc);
		cmd->_arg_capacity = cmd->_arg_capacity * 2;
	}
	else
	{
		cmd->command[cmd->_arg_count] = gc_strdup(str, gc);
		cmd->command[cmd->_arg_count + 1] = NULL;
		cmd->_arg_count++;
	}
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



t_cmd	*parsing(t_token *token, t_minishell *minishell)
{
	t_cmd	*cmd_list;
	t_cmd	*current_cmd;
	t_cmd	*last_cmd;
	int		parenthesis_level;

	cmd_list = NULL;
	current_cmd = new_cmd(minishell);
	last_cmd = NULL;
	parenthesis_level = 0;
	if (check_parsing(token) != SUCCESS)
	{
		print_error(check_parsing(token));
		return (NULL);
	}
	while (token)
	{
		if (token->type == T_WORD)
		{
			add_arg_to_cmd(current_cmd, token->str, minishell->gc);
		}
		else if (token->type == T_REDIR)
		{
			if (last_cmd && last_cmd->logic_operator_type == CLOSE_PARENTHESIS)
				t_redir_parsing(token, &last_cmd, minishell);
			else
				t_redir_parsing(token, &current_cmd, minishell);
			// Avancer de deux tokens : la redirection et son argument
			token = token->next;
			if (token)
				token = token->next;
			continue ;
		}
		else if (token->type == T_PIPE || token->type == T_LOGIC
			|| token->type == T_PARANTHESIS)
		{
			// Mise à jour du niveau de parenthèses
			if (token->type == T_PARANTHESIS)
			{
				if (ft_strcmp(token->str, "(") == 0)
					parenthesis_level++;
				else if (ft_strcmp(token->str, ")") == 0)
					parenthesis_level--;
			}
			// On ajoute la commande courante si elle n'est pas vide
			if (current_cmd->command[0] != NULL || current_cmd->redirs != NULL)
			{
				add_cmd_to_list(&cmd_list, current_cmd);
				last_cmd = current_cmd;
				current_cmd = new_cmd(minishell);
			}
			// On prépare un nœud pour l'opérateur
			t_logic_parsing(token, &current_cmd);
			// On ajoute ce nœud opérateur à la liste
			add_cmd_to_list(&cmd_list, current_cmd);
			last_cmd = current_cmd;
			// On prépare la prochaine commande
			current_cmd = new_cmd(minishell);
		}
		token = token->next;
	}
	// Ajout de la dernière commande si elle n'est pas vide
	if (current_cmd && (current_cmd->command[0] != NULL
			|| current_cmd->redirs != NULL))
	{
		add_cmd_to_list(&cmd_list, current_cmd);
	}
	else if (current_cmd)
	{
		// Libère la mémoire si inutilisée (optionnel selon ton GC)
		// gc_free(current_cmd, minishell->gc);
	}
	return (cmd_list);
}
