/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 17:33:03 by mafioron          #+#    #+#             */
/*   Updated: 2025/03/21 17:18:13 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/token.h"

int	is_operator(char c)
{
	if (c == '<' || c == '>' || c == '|' || c == '&'
			|| c == '(' || c == ')')
		return (1);
	return (0);
}

int	quotes_verif(const char *input)
{
	int	i;
	int	sin_quotes;
	int	doub_quotes;

	i = -1;
	sin_quotes = 0;
	doub_quotes = 0;
	while (input[++i])
	{
		if (input[i] == '\'')
			sin_quotes++;
		if (input[i] == '"')
			doub_quotes++;
	}
	if (!ft_is_even(sin_quotes) || !ft_is_even(doub_quotes))
		return (1);
	return (0);
}

void	finalize_token(t_tokenizer *tok, t_minishell *minishell)
{
	if (tok->nb_tok > 0)
	{
		tok->buff[tok->nb_tok] = '\0';
		add_token(&tok->token_list, tok->buff, T_WORD, tok->token_state, minishell);
	}
}
