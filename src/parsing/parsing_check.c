/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_check.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 16:44:02 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/27 16:54:43 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

enum error_parsing	start_end_check_parsing(t_token *token, int status)
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
		if (token->type == T_REDIR) 
			return (ERR_SYNTAX_REDIRECT);
		if (token->type == T_PIPE)
			return (ERR_SYNTAX_PIPE);
		if (token->type == T_LOGIC)
			return (ERR_SYNTAX_PIPE);
	}
	return (SUCCESS);
}

enum error_parsing	check_parenthesis(t_token *token_p)
{
	t_token	*token;
	int		is_parenthesis;
	int		close_parenthesis;

	token = token_p;
	is_parenthesis = 0;
	close_parenthesis = 0;
	while (token)
	{
		if (token->type == T_PARANTHESIS && ft_strcmp(token->str, "(") == 0)
			is_parenthesis++;
		if (token->type == T_PARANTHESIS && ft_strcmp(token->str, ")") == 0)
			is_parenthesis--;
		if (token->type == T_PARANTHESIS && ft_strcmp(token->str, "(") == 0
			&& close_parenthesis == 0)
			close_parenthesis = -1;
		else if (token->type == T_PARANTHESIS && ft_strcmp(token->str, ")") == 0
			&& close_parenthesis == 0)
			close_parenthesis = 1;
		token = token->next;
	}
	if (is_parenthesis != 0 || close_parenthesis == 1)
		return (ERR_SYNTAX_TOKEN);
	return (SUCCESS);
}

enum error_parsing	check_parsing(t_token *token_p)
{
	t_token	*token;

	token = token_p;
	if (!token)
		return (ERR_SYNTAX_TOKEN);
	if (start_end_check_parsing(token, 0) != SUCCESS)
		return (start_end_check_parsing(token, 0));
	while (token->next)
	{
		if (token->type == T_PIPE && token->next->type == T_PIPE)
			return (ERR_SYNTAX_PIPE);
		else if (token->type == T_REDIR && token->next->type != T_WORD)
			return (ERR_SYNTAX_REDIRECT);
		else if (token->type == T_LOGIC && token->next->type == T_LOGIC)
			return (ERR_SYNTAX_LOGIC);
		token = token->next;
	}
	if (start_end_check_parsing(token, 1) != SUCCESS)
		return (start_end_check_parsing(token, 1));
	if (check_parenthesis(token_p) != SUCCESS)
		return (ERR_SYNTAX_TOKEN);
	return (SUCCESS);
}