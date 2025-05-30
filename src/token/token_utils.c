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

int	is_only_space(const char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] != ' ')
			return (0);
	}
	return (1);
}

void	finalize_token(t_tokenizer *tok, t_minishell *minishell,
		const char *input)
{
	if (tok->nb_tok > 0)
	{
		tok->buff[tok->nb_tok] = '\0';
		add_token(tok, T_WORD, tok->token_state, minishell);
	}
	else if ((tok->nb_tok == 0 && tok->i == 0) || is_only_space(input))
	{
		if (tok->buff)
			tok->buff[0] = '\0';
		add_token(tok, T_WORD, tok->token_state, minishell);
	}
}

int	verif_quotes(const char *input)
{
	int	i;
	int	in_squotes;
	int	in_dquotes;

	in_squotes = 0;
	in_dquotes = 0;
	i = -1;
	if (!input)
		return (0);
	while (input[++i])
	{
		if (input[i] == '\'' && !in_dquotes)
			in_squotes = !in_squotes;
		else if (input[i] == '\"' && !in_squotes)
			in_dquotes = !in_dquotes;
	}
	if (in_dquotes || in_squotes)
		return (1);
	else
		return (0);
}

void	init_tokenizer(t_tokenizer *tok, const char *input,
		t_minishell *minishell)
{
	tok->i = -1;
	tok->nb_tok = 0;
	if (input)
		tok->buff = gc_malloc((sizeof(char) * ft_strlen(input) + 1),
				minishell->gc);
	else
		tok->buff = NULL;
	tok->token_list = NULL;
	tok->state = NORMAL;
	tok->token_state = NORMAL;
}
