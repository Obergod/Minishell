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

int	correct_var(char *var)
{
	int	i;

	i = -1;
	while (var[++i])
	{
		if (!ft_isalnum(var[i]))
			return (0);
	}
	return (1);
}

t_token	*expand_vars(t_token *token)
{
	char	*new_str;
	char	*tmp;

	while (token)
	{
		if (token->type != T_WORD || token->state == IN_SQUOTE
				|| !ft_strchr(token->str, '$'))
			token = token->next
		else
		{
			if (ft_strstr(token->str, "$?"))
				handle_exit_status(token->str,)
			
		}
	}
}


char	*handle_exit_status(char *str)
{
	char	*res;
	char	*temp;

	res = ft_itoa(g_exit_status);
	temp = ft_strchr(str, "$?");
}
