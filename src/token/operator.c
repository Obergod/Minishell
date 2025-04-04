/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 12:43:18 by mafioron          #+#    #+#             */
/*   Updated: 2025/04/04 12:43:21 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

void	process_operator(const char *input, t_tokenizer *tok, t_minishell *minishell)
{
	enum e_token_type	type;

	if (tok->nb_tok > 0)
	{
		tok->buff[tok->nb_tok] = '\0';
		add_token(&tok->token_list, tok->buff, T_WORD, tok->token_state, minishell);
		tok->nb_tok = 0;
		tok->token_state = tok->state;
	}
	operator_str(input, tok->buff, tok->i);
	type = handle_operator(input, &tok->i);
	add_token(&tok->token_list, tok->buff, type, NORMAL, minishell);
}

void	operator_str(const char *input, char *buff, int i)
{
	if (input[i] == '(' || input[i] == ')')
	{
		buff[0] = input[i];
		buff[1] = '\0';
		return ;
	}
	if (is_operator(input[i + 1]) && input[i] == input[i + 1])
	{
		buff[0] = input[i];
		buff[1] = input[i + 1];
		buff[2] = '\0';
	}
	else
	{
		buff[0] = input[i];
		buff[1] = '\0';
	}
}

enum e_token_type	handle_operator(const char *input, int *i)
{
	char	op;
	char	next_char;

	op = input[*i];
	next_char = input[*i + 1];
	if (op == '<')
	{
		if (next_char == '<')
			(*i)++;
		return (T_REDIR);
	}
	else if (op == '>')
	{
		if (next_char == '>')
			(*i)++;
		return (T_REDIR);
	}
	else if (op == '|')
	{
		if (next_char == '|')
		{
			(*i)++;
			return (T_LOGIC);
		}
		return (T_PIPE);
	}
	else if (op == '(' || op == ')')
		return (T_PARANTHESIS);
	else if (op == '&')
	{
		if (next_char == '&')
		{
			(*i)++;
			return (T_LOGIC);
		}
//		exit_and_cleanup;
	}
	return (-1);
}
