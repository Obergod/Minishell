/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 12:43:18 by mafioron          #+#    #+#             */
/*   Updated: 2025/04/05 19:50:37 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/token.h"

int	process_operator(const char *input, t_tokenizer *tok,
		t_minishell *minishell)
{
	enum e_token_type	type;

	if (verif_op(input, tok->i) == 1)
		return (1);
	if (tok->nb_tok > 0)
	{
		tok->buff[tok->nb_tok] = '\0';
		add_token(tok, T_WORD, tok->token_state, minishell);
		tok->nb_tok = 0;
		tok->token_state = tok->state;
	}
	operator_str(input, tok->buff, tok->i);
	type = handle_operator(input, &tok->i);
	add_token(tok, type, NORMAL, minishell);
	return (0);
}

int	verif_op(const char *input, int i)
{
	if (is_operator(input[i + 1]) && input[i] != input[i + 1])
		return (1);
	if (input[i + 2] && is_operator(input[i + 2]))
		return (1);
	if (input[i] == '&' && input[i + 1] && input[i + 1] != input[i])
		return (1);
	return (0);
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
	if (is_redir(op, next_char, i) == 1)
		return (T_REDIR);
	else if (op == '(' || op == ')')
		return (T_PARANTHESIS);
	else if (is_pipe_or_or(op, next_char, i) == 1)
		return (T_PIPE);
	else if (is_pipe_or_or(op, next_char, i) == 2)
		return (T_LOGIC);
	else if (op == '&')
	{
		if (next_char == '&')
		{
			(*i)++;
			return (T_LOGIC);
		}
	}
	return (-1);
}
