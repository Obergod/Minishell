/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_remove.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 18:45:48 by ufalzone          #+#    #+#             */
/*   Updated: 2025/05/15 19:05:02 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/expansion.h"
#include "../../includes/parsing.h"

int	remove_quotes_redirs(t_redir *redirs, t_minishell *minishell)
{
	struct s_redir	*cur;
	char			*new_str;

	cur = redirs;
	while (cur)
	{
		new_str = remove_quotes(cur->file_or_delimiter, minishell);
		if (!new_str)
			return (-1);
		cur->file_or_delimiter = new_str;
		cur = cur->next;
	}
	return (0);
}

char	**remove_quotes_array(char **tokens, t_minishell *minishell)
{
	char	*new_str;
	int		i;

	i = -1;
	while (tokens[++i])
	{
		new_str = remove_quotes(tokens[i], minishell);
		if (new_str && new_str != tokens[i])
			tokens[i] = new_str;
	}
	return (tokens);
}

char	*remove_quotes(char *str, t_minishell *minishell)
{
	char			*new_str;
	int				i;
	int				j;
	t_quotes_state	quotes;

	i = 0;
	j = 0;
	quotes.in_squotes = 0;
	quotes.in_dquotes = 0;
	new_str = gc_calloc(ft_strlen(str) + 1, sizeof(char), minishell->gc);
	if (!new_str)
		return (NULL);
	while (str[i])
	{
		manage_quote_states(str, &i, &quotes);
		if (should_keep_char(str[i], quotes))
			new_str[j++] = str[i];
		i++;
	}
	return (new_str);
}
