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
#include "expand.h"

/* Global variable definition for the exit status */
int g_exit_status = 0;

t_token	*expand_vars(t_token *token, t_minishell *minishell)
{
	char	*new_str;
	t_token	*head;

	head = token;
	while (token)
	{
		if (token->type != T_WORD || token->state == IN_SQUOTE
				|| !ft_strchr(token->str, '$'))
			token = token->next;
		else
		{
			new_str = expand_str(token->str, minishell);
			if (!new_str)
			//	cleanup_and_exit();
			free(token->str);
			token->str = new_str;
			token = token->next;
		}
	}
	return (head);
}

char	*expand_str(char *str, t_minishell *minishell)
{
	char	*full_var;
	int		full_len;
	char	*new_str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	full_len = get_full_len(str, minishell);
	if (full_len < 0)
		return (NULL);
	new_str = ft_calloc(full_len + 1, sizeof(char));
	if (!new_str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
		{
			i++;
			full_var = get_vars(str, &i, minishell);
			if (!full_var)
				return (NULL);
			ft_strlcpy(new_str + j, full_var, ft_strlen(full_var));
			j += ft_strlen(full_var);
			free(full_var);
		}
		else
			new_str[j++] = str[i++];
	}
	return (new_str);
}

char	*get_vars(char *str, int *i, t_minishell *minishell)
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
	res = find_in_env(var_name, minishell);
	free(var_name);
	if (!res)
		return (ft_strdup(""));
	return (res);
}

int	get_full_len(char *str, t_minishell *minishell)
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
			full_var = get_vars(str, &i, minishell);
			if (!full_var)
				return (0);
			full_len += ft_strlen(full_var);
		}
		else
		{
			i++;
			full_len++;
		}
	}
	return (full_len);
}
