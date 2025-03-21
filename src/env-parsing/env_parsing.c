/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 16:48:21 by ufalzone          #+#    #+#             */
/*   Updated: 2025/03/21 16:14:47 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/env_parsing.h"

static int init_head_env_parsing(char **envp, t_minishell *minishell, t_env **current, t_env **head)
{
	char **result_split;

	*head = gc_malloc(sizeof(t_env), minishell->gc);
	if (!*head)
		return (1);
	result_split = gc_split(envp[0], '=', minishell->gc);
	if (!result_split)
		return (1);
	(*head)->key = result_split[0];
	(*head)->value = result_split[1];
	(*head)->next = NULL;
	*current = *head;
	return (0);
}

#include <stdlib.h>

t_env *env_parsing(char **envp, t_minishell *minishell)
{
	t_env *current;
	t_env *tmp;
	char **result_split;
	int i;
	t_env *head;

	if (init_head_env_parsing(envp, minishell, &current, &head) == 1)
		return (NULL);
	i = 0;
	while (envp[++i])
	{
		gc_alloc(result_split = gc_split(envp[i], '=', minishell->gc), minishell->gc);
		if (!result_split)
			return (NULL);
		tmp = gc_malloc(sizeof(t_env), minishell->gc);
		if (!tmp)
			return (NULL);
		tmp->key = result_split[0];
		tmp->value = result_split[1];
		tmp->raw = gc_strjoin_three(result_split[0], "=", result_split[1], minishell->gc);
		tmp->next = NULL;
		current->next = tmp;
		current = current->next;
	}
	return (head);
}

char *find_in_env(char *key, t_minishell *minishell)
{
	t_env *current;

	current = minishell->env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			printf("%s\n", current->raw);
			return (current->value);
		}
		current = current->next;
	}
	return (NULL);
}

int main(int ac, char **av, char **envp)
{
	t_minishell minishell;
	t_gc_head *gc_head;
	char *result;

	(void)ac; // Pour éviter l'avertissement de paramètre non utilisé
	gc_head = gc_init();
	minishell.gc = gc_head;
	minishell.env = env_parsing(envp, &minishell);
	// aff_env(minishell.env);
	if ((result = find_in_env(av[1], &minishell)) != NULL)
			printf("%s\n", result);

	gc_print(gc_head);
	gc_destroy(gc_head);
}
