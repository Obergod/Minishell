/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parsing_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 15:26:47 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/27 15:31:27 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/env_parsing.h"

int	init_head_env_parsing(char **envp, t_minishell *minishell, t_env **current,
		t_env **head)
{
	char	**result_split;

	if (!envp || !envp[0])
		return (1);
	*head = gc_malloc(sizeof(t_env), minishell->gc);
	if (!*head)
		return (1);
	result_split = gc_split(envp[0], '=', minishell->gc);
	if (!result_split || !result_split[0])
		return (1);
	(*head)->key = result_split[0];
	if (!result_split[1])
		result_split[1] = "";
	(*head)->value = result_split[1];
	(*head)->raw = gc_strjoin_three(result_split[0], "=", result_split[1],
			minishell->gc);
	(*head)->next = NULL;
	*current = *head;
	return (0);
}

void	add_to_env(t_env **head, t_env *current)
{
	t_env	*tmp;

	if (*head == NULL)
	{
		*head = current;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = current;
}

t_env	*create_value_env(char *key, char *value, t_minishell *minishell)
{
	t_env	*tmp;

	tmp = gc_malloc(sizeof(t_env), minishell->gc);
	tmp->key = key;
	tmp->value = value;
	if (!tmp->value)
		tmp->value = "";
	tmp->raw = gc_strjoin_three(tmp->key, "=", tmp->value, minishell->gc);
	tmp->next = NULL;
	return (tmp);
}
