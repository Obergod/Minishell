/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:16:27 by mafioron          #+#    #+#             */
/*   Updated: 2025/04/05 19:57:37 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/expand.h"

//gerer l'export
/*
t_token	*expand_vars(t_token *token, t_minishell *minishell)
{
	char	*new_str;
	t_token	*head;

	head = token;
	while (token)
	{
		if (token->type == T_WORD && token->state != IN_SQUOTE)
		{
			if (ft_strchr(token->str, '$'))
			{
				new_str = expand_str(token->str, minishell);
				if (!new_str)
					return (NULL); //temporaire pour pas d'erreur
				//	cleanup_and_exit();
				free(token->str);
				token->str = new_str;
			}
//			if (ft_strchr(token->str, '*'))
//				expand_wildcards()
			token = token->next;
		}
		else
			token = token->next;
	}
	return (head);
}
*/

char	**expand_vars(char **cmd, t_minishell *minishell)
{
	char	*new_str;
	char	**res;
	int	i;

	i = 0;
	while (cmd[i])
		i++;
	res = malloc(sizeof(char *) * i + 1);
	i = 0;
	while (cmd[i])
	{
		if (ft_strchr(cmd[i], '$'))
		{
			new_str = expand_str(cmd[i], minishell);
			if (!new_str)
				return (NULL); //temporaire pour pas d'erreur
			res[i] = new_str;
		}
		else
			res[i] = cmd[i];
//			if (ft_strchr(token->str, '*'))
//				expand_wildcards()
		i++;
	}
	res[i] = 0;
	return (res);
}

char	*expand_str(char *str, t_minishell *minishell)
{
	char	*full_var;
	int		full_len;
	char	*new_str;
	int		i;
	int		j;
	int		in_squotes = 0;
	int		in_dquotes = 0;

	i = 0;
	j = 0;
	full_len = get_full_len(str, minishell);
	if (full_len < 0)
		return (NULL);
	new_str = gc_calloc(full_len + 1, sizeof(char), minishell->gc);
	if (!new_str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\'' && !in_dquotes)
		{
			in_squotes = !in_squotes;
			new_str[j++] = str[i++];
		}
		else if (str[i] == '\"' && !in_squotes)
		{
			in_dquotes = !in_dquotes;
			new_str[j++] = str[i++];
		}
		else if (str[i] == '$' && !in_squotes)
		{
			i++;
			full_var = get_vars(str, &i, minishell);
			if (!full_var)
				return (NULL);
			ft_strlcpy(new_str + j, full_var, ft_strlen(full_var) + 1);
			j += ft_strlen(full_var);
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
		return (gc_strdup("$", minishell->gc));
	if (str[*i] == '?')
	{
		(*i)++;
		res = ft_itoa(minishell->exit_status);
		if (!res)
			return (NULL);
		return (res);
	}
	while (ft_isalnum(str[*i]) || str[*i] == '_')
		(*i)++;
	var_name = ft_substr(str, start, *i - start);
	if (!var_name)
		return(NULL);
	res = find_in_env(var_name, minishell);
	if (!res)
		return (gc_strjoin("$", var_name, minishell->gc));
	free(var_name);
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
