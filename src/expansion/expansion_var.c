/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_var.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 18:43:11 by ufalzone          #+#    #+#             */
/*   Updated: 2025/05/15 19:11:36 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ast.h"
#include "../../includes/env_parsing.h"
#include "../../includes/exec.h"
#include "../../includes/expansion.h"

char	*get_var_name(char *str, int *i)
{
	int		start;
	char	*var_name;

	start = *i;
	while (ft_isalnum(str[*i]) || str[*i] == '_')
		(*i)++;
	var_name = ft_substr(str, start, *i - start);
	return (var_name);
}

char	*get_vars(char *str, int *i, struct s_minishell *minishell)
{
	char	*res;
	char	*var_name;

	var_name = NULL;
	res = NULL;
	if (str[*i] != '?' && !ft_isalpha(str[*i]) && str[*i] != '_')
		return (gc_strdup("$", minishell->gc));
	if (str[*i] == '?')
		return (get_exit_status(i, minishell));
	var_name = get_var_name(str, i);
	if (!var_name)
		return (NULL);
	res = find_in_env(var_name, minishell);
	if (!res)
	{
		free(var_name);
		return (gc_strdup("", minishell->gc));
	}
	free(var_name);
	return (res);
}

int	process_var_len(char *str, int *i, struct s_minishell *minishell)
{
	char	*full_var;
	int		len;

	full_var = get_vars(str, i, minishell);
	if (!full_var)
		return (0);
	len = ft_strlen(full_var);
	return (len);
}

int	get_full_len(char *str, struct s_minishell *minishell)
{
	int	i;
	int	full_len;
	int	var_len;

	i = 0;
	full_len = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
		{
			i++;
			var_len = process_var_len(str, &i, minishell);
			if (var_len == 0)
				return (0);
			full_len += var_len;
		}
		else
		{
			i++;
			full_len++;
		}
	}
	return (full_len);
}

char	**expand_vars(char **cmd, struct s_minishell *minishell)
{
	char	**res;
	int		i;
	int		count;

	i = -1;
	count = count_cmd_tokens(cmd);
	res = gc_malloc(sizeof(char *) * (count + 1), minishell->gc);
	if (!res)
		return (NULL);
	while (++i < count)
	{
		res[i] = process_token(cmd[i], minishell);
		if (!res[i])
			return (NULL);
		res[i] = expand_wildcard_if_needed(cmd[i], res[i], minishell);
	}
	res[i] = NULL;
	res = remove_quotes_array(res, minishell);
	return (check_empty(res, minishell));
}
