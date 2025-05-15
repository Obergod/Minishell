/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 18:44:22 by ufalzone          #+#    #+#             */
/*   Updated: 2025/05/15 18:56:53 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/expansion.h"

void	copy_var_value(char *var, t_exp_data *data)
{
	int	len;

	if (!var)
		return ;
	len = ft_strlen(var);
	ft_strlcpy(data->new_str + data->j, var, len + 1);
	data->j += len;
}

int	count_cmd_tokens(char **cmd)
{
	int	count;

	count = 0;
	while (cmd[count])
		count++;
	return (count);
}

char	*dup_token(char *token, t_minishell *minishell)
{
	char	*result;

	result = gc_strdup(token, minishell->gc);
	if (!result)
		return (NULL);
	return (result);
}

char	*expand_wildcard_if_needed(char *token, char *expanded_token,
		t_minishell *minishell)
{
	char	*result;

	if (ft_strchr(token, '*'))
	{
		result = expand_wildcards(token, minishell);
		if (result)
			return (result);
	}
	return (expanded_token);
}

char	*get_exit_status(int *i, t_minishell *minishell)
{
	char	*res;

	(*i)++;
	res = gc_itoa(minishell->exit_status, minishell->gc);
	if (!res)
		return (NULL);
	return (res);
}
