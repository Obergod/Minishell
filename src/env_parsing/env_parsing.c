/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 16:48:21 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/04 19:33:17 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/env_parsing.h"
#include <unistd.h>

static int init_head_env_parsing(char **envp, t_minishell *minishell, t_env **current, t_env **head)
{
	char **result_split;

	if (!envp || !envp[0])
		return (1);

	*head = gc_malloc(sizeof(t_env), minishell->gc);
	if (!*head)
		return (1);
	result_split = gc_split(envp[0], '=', minishell->gc);
	if (!result_split)
		return (1);
	if (!result_split[0])
		return (1);
	(*head)->key = result_split[0];
	(*head)->value = result_split[1];
	(*head)->raw = gc_strjoin_three(result_split[0], "=", result_split[1] ? result_split[1] : "", minishell->gc);
	(*head)->next = NULL;
	*current = *head;
	return (0);
}

#include <stdlib.h>

static void add_to_env(t_env **head, t_env *current)
{
	t_env *tmp;

	if (*head == NULL)
	{
		*head = current;
		return;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = current;
}

static t_env *create_value_env(char *key, char *value, t_minishell *minishell)
{
	t_env *tmp;

	tmp = gc_malloc(sizeof(t_env), minishell->gc);
	tmp->key = key;
	tmp->value = value;
	tmp->raw = gc_strjoin_three(tmp->key, "=", tmp->value ? tmp->value : "", minishell->gc);
	return (tmp);
}

//appelle cette fonction si lancee avec env -i
t_env *init_env(t_minishell *minishell)
{
	t_env *head;
	t_env *current;

	current = create_value_env("PWD", getcwd(NULL, 0), minishell);
	add_to_env(&head, current);
	current = create_value_env("SHLVL", "1", minishell);
	add_to_env(&head, current);
	current = create_value_env("_", "/usr/bin/env", minishell);
	add_to_env(&head, current);
	return (head);
}

//il faut dans tous les cas avoir _=/usr/bin/env, SHLVL, PWD
//check[0] = PWD
//check[1] = SHLVL
//check[2] = _
void check_env(t_env *head, t_minishell *minishell)
{
	t_env *current;
	int check[3];

	current = head;
	check[0] = 0;
	check[1] = 0;
	check[2] = 0;
	while (current)
	{
		if (ft_strcmp("PWD", current->key) == 0)
			check[0] = 1;
		if (ft_strcmp("SHLVL", current->key) == 0)
			check[1] = 1;
		if (ft_strcmp("_", current->key) == 0)
			check[2] = 1;
		current = current->next;
	}
	if (check[0] == 0)
		add_to_env(&head, create_value_env("PWD", getcwd(NULL, 0), minishell));
	if (check[1] == 0)
		add_to_env(&head, create_value_env("SHLVL", "1", minishell));
	if (check[2] == 0)
		add_to_env(&head, create_value_env("_", "/usr/bin/env", minishell));
}

t_env *env_parsing(char **envp, t_minishell *minishell)
{
	t_env *current;
	t_env *tmp;
	char **result_split;
	int i;
	t_env *head;

	if (!envp || !envp[0])
		return (init_env(minishell));

	if (init_head_env_parsing(envp, minishell, &current, &head) == 1)
		return (NULL);

	i = 0;
	while (envp[++i])
	{
		result_split = gc_split(envp[i], '=', minishell->gc);
		if (!result_split)
			return (NULL);

		tmp = gc_malloc(sizeof(t_env), minishell->gc);
		if (!tmp)
			return (NULL);

		tmp->key = result_split[0];
		tmp->value = result_split[1] ? result_split[1] : "";
		tmp->raw = gc_strjoin_three(result_split[0], "=", result_split[1] ? result_split[1] : "", minishell->gc);
		tmp->next = NULL;
		current->next = tmp;
		current = current->next;
	}
	check_env(head, minishell);
	return (head);
}

char *find_in_env(char *key, t_minishell *minishell)
{
	t_env *current;

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

// int main(int ac, char **av, char **envp)
// {
// 	t_minishell minishell;
// 	t_gc_head *gc_head;
// 	char *result;

// 	(void)ac; // Pour éviter l'avertissement de paramètre non utilisé
// 	gc_head = gc_init();
// 	minishell.gc = gc_head;
// 	minishell.env = env_parsing(envp, &minishell);
// 	// aff_env(minishell.env);
// 	if ((result = find_in_env(av[1], &minishell)) != NULL)
// 			printf("%s\n", result);

// 	gc_print(gc_head);
// 	gc_destroy(gc_head);
// }
