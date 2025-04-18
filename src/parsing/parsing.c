/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:01:47 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/06 19:14:22 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"
#include "../../includes/token.h"
#include <string.h>

//<< en position 0
//EOF sera en position 1
//cmd en position 2

//< en position 0
//infile en position 1
//cmd en position 2

//aucun operator en 0
//commande sera en 0

//status : 0 = debut
//status : 1 = fin
static enum error_parsing start_end_check_parsing(t_token *token, int status)
{
	if (status == 0)
	{
		if (token->type == T_PIPE)
			return (ERR_SYNTAX_PIPE);
		if (token->type == T_LOGIC)
			return (ERR_SYNTAX_LOGIC);
	}
	else if (status == 1)
	{
		if (token->type == T_REDIR) //si ca se finit par une redirection
			return (ERR_SYNTAX_REDIRECT);
		if (token->type == T_PIPE) //si ca se finit par un pipe
			return (ERR_SYNTAX_PIPE);
		if (token->type == T_LOGIC) //si ca se finit par un logic
			return (ERR_SYNTAX_PIPE);
	}
	return (SUCCESS);
}

enum error_parsing check_parsing(t_token *token_p)
{
	t_token *token;

	token = token_p;
	if (!token)
		return (ERR_SYNTAX_TOKEN);
	if (start_end_check_parsing(token, 0) != SUCCESS)
		return (start_end_check_parsing(token, 0));
	while (token->next)
	{
		if (token->next && token->type == T_PIPE && token->next->type == T_PIPE)
			return (ERR_SYNTAX_PIPE);
		if (token->next && token->type == T_REDIR && token->next->type != T_WORD)
			return (ERR_SYNTAX_REDIRECT);
		if (token->next && token->type == T_LOGIC && token->next->type == T_LOGIC)
			return (ERR_SYNTAX_LOGIC);
		token = token->next;
	}
	//on est au dernier token de la liste
	if (start_end_check_parsing(token, 1) != SUCCESS)
		return (start_end_check_parsing(token, 1));
	return (SUCCESS);
}


t_cmd	*new_cmd(t_minishell *minishell)
{
	t_cmd *new;
	int i;

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

t_redir *new_redir(enum e_redir_type type, char *file_or_delimiter, t_gc_head *gc)
{
	t_redir *redir;

	redir = gc_malloc(sizeof(t_redir), gc);
	redir->type = type;
	redir->file_or_delimiter = gc_strdup(file_or_delimiter, gc);
	redir->next = NULL;
	return (redir);
}

void add_redir_to_cmd(t_cmd *cmd, t_redir *redir)
{
	t_redir *tmp;

	if (cmd->redirs == NULL)
	{
		cmd->redirs = redir;
		return;
	}
	tmp = cmd->redirs;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = redir;
}

//creer command->size = 4
//si jai mit 3 arg dans command => arg_count = 3
//


void add_arg_to_cmd(t_cmd *cmd, char *str, t_gc_head *gc)
{
	if (cmd->command_raw == NULL)
		cmd->command_raw = gc_strdup(str, gc);
	else
		cmd->command_raw = gc_strjoin_three(cmd->command_raw, " ", str, gc);
	if (cmd->_arg_count >= cmd->_arg_capacity - 1)
	{
		cmd->command = gc_strndup_array(cmd->command, cmd->_arg_capacity * 2, gc);
		cmd->_arg_capacity = cmd->_arg_capacity * 2;

	} else {
		cmd->command[cmd->_arg_count] = gc_strdup(str, gc);
		cmd->command[cmd->_arg_count + 1] = NULL;
		cmd->_arg_count++;
	}
}

void add_cmd_to_list(t_cmd **cmd_list, t_cmd *current)
{
	t_cmd *tmp;

	if (*cmd_list == NULL)
	{
		*cmd_list = current;
		return;
	}
	tmp = *cmd_list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = current;
}

void print_error(enum error_parsing error)
{
    switch (error)
    {
        case SUCCESS:
            printf("SUCCESS\n");
            break;
        case ERR_SYNTAX_REDIRECT:
            printf("ERR_SYNTAX_NEWLINE\n");
            break;
        case ERR_SYNTAX_PIPE:
            printf("ERR_SYNTAX_PIPE\n");
            break;
        case ERR_SYNTAX_TOKEN:
            printf("ERR_SYNTAX_TOKEN\n");
            break;
		case ERR_SYNTAX_LOGIC:
			printf("ERR_SYNTAX_LOGIC");
			break;
    }
}

static void t_redir_parsing(t_token *token, t_cmd **current_cmd, t_minishell *minishell)
{
	t_redir *redir;

	if (ft_strcmp(token->str, "<") == 0)
	{
		redir = new_redir(REDIR_IN, token->next->str, minishell->gc);
		add_redir_to_cmd(*current_cmd, redir);
	}
	else if(ft_strcmp(token->str, ">") == 0)
	{
		redir = new_redir(REDIR_OUT, token->next->str, minishell->gc);
		add_redir_to_cmd(*current_cmd, redir);
	}
	else if(ft_strcmp(token->str, ">>") == 0)
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

static void t_logic_parsing(t_token *token, t_cmd **current_cmd)
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

t_cmd *parsing(t_token *token, t_minishell *minishell)
{
	t_cmd *cmd_list;
	t_cmd *current_cmd;
	int position;
	int parenthesis_level;
	t_token *prev_token;
	t_cmd *prev_cmd; // Pour garder une trace du token précédent

	cmd_list = NULL;
	prev_token = NULL;
	current_cmd = new_cmd(minishell);
	position = 0;
	parenthesis_level = 0;

	if (check_parsing(token) != SUCCESS)
	{
		print_error(check_parsing(token));
		return (NULL);
	}
	while (token)
	{
		if (token->type == T_WORD)
			add_arg_to_cmd(current_cmd, token->str, minishell->gc);
		else if (token->type == T_REDIR)
		{
			// di on est apres une parenthese fermante et que parenthesis_level == 0,
			// alors les redirections s'appliquent a toute la sous-commande precedente
			if (position > 0 && parenthesis_level == 0 &&
				prev_token && prev_token->type == T_PARANTHESIS &&
				ft_strcmp(prev_token->str, ")") == 0)
			{
				// Trouver la commande précédente qui a reçu la parenthèse fermante
				prev_cmd = cmd_list;
				while (prev_cmd && prev_cmd->next)
					prev_cmd = prev_cmd->next;

				// Appliquer la redirection à cette commande
				if (prev_cmd)
				{
					// Stocker la redirection dans la commande précédente
					t_redir_parsing(token, &prev_cmd, minishell);
				}
				else
				{
					// Si pas de commande précédente (ne devrait pas arriver),
					// le traiter normalement
					t_redir_parsing(token, &current_cmd, minishell);
				}
			}
			else
			{
				// Comportement normal pour les redirections
				t_redir_parsing(token, &current_cmd, minishell);
			}
			prev_token = token; // Mémoriser le token actuel
			token = token->next;
		}
		else if (token->type == T_PIPE || token->type == T_LOGIC || token->type == T_PARANTHESIS)
		{
			// Gérer le niveau de parenthèses
			if (token->type == T_PARANTHESIS)
			{
				if (ft_strcmp(token->str, "(") == 0)
					parenthesis_level++;
				else if (ft_strcmp(token->str, ")") == 0)
					parenthesis_level--;
			}

			// Ajouter la commande courante à la liste seulement si elle contient quelque chose
			if (current_cmd->command[0] != NULL || current_cmd->redirs != NULL)
			{
				add_cmd_to_list(&cmd_list, current_cmd);
				current_cmd = new_cmd(minishell);
			}

			// Créer un nœud spécifique pour l'opérateur
			if (token->type == T_PIPE || token->type == T_PARANTHESIS || token->type == T_LOGIC)
				t_logic_parsing(token, &current_cmd);
			add_cmd_to_list(&cmd_list, current_cmd);

			if (token->next != NULL)
				current_cmd = new_cmd(minishell);
			else
				current_cmd = NULL;

			position++;
			prev_token = token; // Mémoriser le token actuel
		}
		else
		{
			prev_token = token; // Mémoriser le token actuel
		}
		token = token->next;
	}
	if (current_cmd != NULL && (current_cmd->command[0] != NULL || current_cmd->redirs != NULL))
		add_cmd_to_list(&cmd_list, current_cmd);
	return (cmd_list);
}

