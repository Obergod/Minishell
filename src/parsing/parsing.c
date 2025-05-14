/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:01:47 by ufalzone          #+#    #+#             */
/*   Updated: 2025/05/14 14:18:27 by ufalzone         ###   ########.fr       */
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

static void	parsing_loop(t_token *token, t_cmd **current, t_cmd **head,
		t_minishell *minishell)
{
	while (token)
	{
		if (token->type == T_WORD)
			add_arg_to_cmd(*current, token->str, minishell->gc);
		else if (token->type == T_REDIR)
		{
			t_redir_parsing(token, &(*current), minishell);
			token = token->next;
		}
		else
			handle_parsing_operator(token, &(*head), &(*current), minishell);
		token = token->next;
	}
}

t_cmd	*parsing(t_token *token, t_minishell *minishell)
{
	t_cmd	*current;
	t_cmd	*head;
	int		status;

	current = new_cmd(minishell);
	head = NULL;
	// status = check_parsing(token);
	// if (status != SUCCESS)
	// 	return (print_error(status), NULL);
	parsing_loop(token, &current, &head, minishell);
	if (current->command[0] || current->redirs)
		add_cmd_to_list(&head, current);
	return (head);
}
