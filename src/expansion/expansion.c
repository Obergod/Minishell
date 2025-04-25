/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:16:27 by mafioron          #+#    #+#             */
/*   Updated: 2025/04/25 16:34:32 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/main.h"


char	**expand_vars(char **cmd, t_minishell *minishell)
{
	char	*new_str;
	char	**res;
	int		i;
	int		count;

	i = -1;
	count = 0;
	while (cmd[count])
		count++;
	res = gc_malloc(sizeof(char *) * (count + 1), minishell->gc);
	if (!res)
		return (NULL);
	while (++i < count)
	{
		if (ft_strchr(cmd[i], '$'))
		{
			new_str = expand_str(cmd[i], minishell);
			if (new_str)
				res[i] = new_str;
			else
			{
				res[i] = gc_strdup(cmd[i], minishell->gc);
				if (!res[i])
					return (NULL);
			}
		}
		else
		{
			res[i] = gc_strdup(cmd[i], minishell->gc);
			if (!res[i])
				return (NULL);
		}
		if (ft_strchr(cmd[i], '*'))
		{
			new_str = expand_wildcards(cmd[i], minishell);
			if (new_str)
				res[i] = new_str;
		}
	}
	res[i] = NULL;
	i = -1;
	while (res[++i])
	{
		new_str = remove_quotes(res[i], minishell);
		if (new_str && new_str != res[i])
			res[i] = new_str;
	}
	return (check_empty(res, minishell));
}

int	remove_quotes_redirs(t_redir *redirs, t_minishell *minishell)
{
	t_redir *cur;
	char	*new_str;

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

char	*remove_quotes(char *str, t_minishell *minishell)
{
	char	*new_str;
	int		i;
	int		j;
	int		in_squotes = 0;
	int		in_dquotes = 0;
	
	i = -1;
	j = 0;
	new_str = gc_calloc(ft_strlen(str) + 1, sizeof(char), minishell->gc);
	if (!new_str)
		return (NULL);
	while (str[++i])
	{
		if (str[i] == '\'' && !in_dquotes)
			in_squotes = !in_squotes;
		else if (str[i] == '\"' && !in_squotes)
			in_dquotes = !in_dquotes;
		else
			new_str[j++] = str[i];
	}
	return (new_str);
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
	if (str[*i] != '?' && !ft_isalpha(str[*i]) && str[*i] != '_')
		return (gc_strdup("$", minishell->gc));
	if (str[*i] == '?')
	{
		(*i)++;
		res = gc_itoa(minishell->exit_status, minishell->gc);
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
		return (gc_strdup("", minishell->gc));
//		return (gc_strjoin("$", var_name, minishell->gc));
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
