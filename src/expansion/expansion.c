/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:16:27 by mafioron          #+#    #+#             */
/*   Updated: 2025/05/15 19:05:16 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/expansion.h"

void	manage_quote_states(char *str, int *i, t_quotes_state *quotes)
{
	if (str[*i] == '\'' && !(quotes->in_dquotes))
		quotes->in_squotes = !(quotes->in_squotes);
	else if (str[*i] == '\"' && !(quotes->in_squotes))
		quotes->in_dquotes = !(quotes->in_dquotes);
}

int	should_keep_char(char c, t_quotes_state quotes)
{
	return (!(c == '\'' && !quotes.in_dquotes) && !(c == '\"'
			&& !quotes.in_squotes));
}

t_exp_data	init_exp_data(int full_len, t_minishell *minishell)
{
	t_exp_data	data;

	data.i = 0;
	data.j = 0;
	data.quotes.in_squotes = 0;
	data.quotes.in_dquotes = 0;
	data.new_str = gc_calloc(full_len + 1, sizeof(char), minishell->gc);
	return (data);
}

char	*expand_str(char *str, t_minishell *minishell)
{
	t_exp_data	data;
	int			full_len;

	full_len = get_full_len(str, minishell);
	if (full_len < 0)
		return (NULL);
	data = init_exp_data(full_len, minishell);
	if (!data.new_str)
		return (NULL);
	while (str[data.i])
		process_dollar(str, &data, minishell);
	return (data.new_str);
}
