/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafioron <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:16:27 by mafioron          #+#    #+#             */
/*   Updated: 2025/03/19 15:16:27 by mafioron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

t_token	*expand_vars(t_token *token)
{
	char	*new_str;
	char	*full_var;
	int		full_len;

	while (token)
	{
		if (token->type != T_WORD || token->state == IN_SQUOTE
				|| !ft_strchr(token->str, '$'))
			token = token->next
		else
		{
			full_len = get_full_len()
			
		}
	}
}



char	*get_vars(char *str, int *i)
{
	char	*res;
	int		start;
	char	*var_name;

	start = *i;
	var_name = NULL;
	res = NULL;
	if (str[*i] != '?' && !ft_isalpha(str[*i]))
		return (ft_strdup(""));
	if (str[*i] == '?')
	{
		(*i)++;
		res = ft_itoa(g_exit_status);
		if (!res)
			return (NULL);
		return (res);
	}
	while (ft_isalnum(str[*i]) || str[*i] == '_')
		(*i)++;
	var_name = ft_substr(str, start, *i - start);
	if (!var_name)
		return(NULL);
	//check what get_env returns to know if its a malloc_failure or an unsets var
	res = get_env(var_name);
	free(var_name);
	if (!res)
		return (NULL);
	return (res);
}

int	get_full_len(char *str)
{
	int		i;
	int		full_len;
	char	*full_var;

	i = 0;
	full_len = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
		{
			i++;
			full_var = get_var(str, &i);
			if (!full_var)
				return (NULL);
			full_len += ft_strlen(res);
		}
		else
		{
			i++;
			full_len++;
		}
	}
	return (full_len);
}
