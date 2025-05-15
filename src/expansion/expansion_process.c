/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_process.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 18:47:28 by ufalzone          #+#    #+#             */
/*   Updated: 2025/05/15 19:11:36 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/main.h"

char	*process_token(char *token, struct s_minishell *minishell)
{
	char	*new_str;

	if (ft_strchr(token, '$'))
	{
		new_str = expand_str(token, minishell);
		if (new_str)
			return (new_str);
		return (dup_token(token, minishell));
	}
	return (dup_token(token, minishell));
}

void	process_str_char(char *str, t_exp_data *data)
{
	if (str[data->i] == '\'' && !(data->quotes.in_dquotes))
	{
		data->quotes.in_squotes = !(data->quotes.in_squotes);
		data->new_str[(data->j)++] = str[(data->i)++];
	}
	else if (str[data->i] == '\"' && !(data->quotes.in_squotes))
	{
		data->quotes.in_dquotes = !(data->quotes.in_dquotes);
		data->new_str[(data->j)++] = str[(data->i)++];
	}
	else
		data->new_str[(data->j)++] = str[(data->i)++];
}

void	process_dollar(char *str, t_exp_data *data,
		struct s_minishell *minishell)
{
	char	*full_var;

	if (str[data->i] == '$' && !(data->quotes.in_squotes))
	{
		(data->i)++;
		full_var = get_vars(str, &data->i, minishell);
		copy_var_value(full_var, data);
	}
	else
		process_str_char(str, data);
}
