/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 17:33:03 by mafioron          #+#    #+#             */
/*   Updated: 2025/03/06 17:33:04 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

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
