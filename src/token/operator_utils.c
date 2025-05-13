/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:41:01 by mafioron          #+#    #+#             */
/*   Updated: 2025/05/13 16:41:03 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/token.h"

int	is_redir(char op, char next_char, int *i)
{
	if (op == '<')
	{
		if (next_char == '<')
			(*i)++;
		return (1);
	}
	else if (op == '>')
	{
		if (next_char == '>')
			(*i)++;
		return (1);
	}
	return (0);
}

int	is_pipe_or_or(char op, char next_char, int *i)
{
	if (op == '|')
	{
		if (next_char == '|')
		{
			(*i)++;
			return (2);
		}
		return (1);
	}
	return (0);
}
