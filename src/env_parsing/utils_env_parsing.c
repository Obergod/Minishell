/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env_parsing.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 15:22:33 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/27 15:31:30 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/env_parsing.h"

char	**convert_t_env_to_array(t_minishell *minishell)
{
	char	**result;
	t_env	*current;
	int		size;

	current = minishell->env;
	size = 0;
	while (current)
	{
		size++;
		current = current->next;
	}
	result = gc_malloc((sizeof(char *) * size) + 1, minishell->gc);
	if (!result)
		return (NULL);
	current = minishell->env;
	size = 0;
	while (current)
	{
		result[size] = current->raw;
		size++;
		current = current->next;
	}
	result[size - 1] = NULL;
	return (result);
}

int	edit_in_env(char *key, char *new_value, t_minishell *minishell)
{
	t_env	*current;

	if (!key || !minishell || !minishell->env)
		return (1);
	current = minishell->env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			current->value = gc_strdup(new_value, minishell->gc);
			current->raw = gc_strjoin_three(current->key, "=", new_value,
					minishell->gc);
			if (!current->value || !current->raw)
				return (1);
			return (0);
		}
		current = current->next;
	}
	add_to_env(&minishell->env, create_value_env(key, new_value, minishell));
	return (2);
}

char	*find_in_env(char *key, t_minishell *minishell)
{
	t_env	*current;

	if (!key || !minishell || !minishell->env)
		return (NULL);
	current = minishell->env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			return (current->value);
		}
		current = current->next;
	}
	return (NULL);
}
