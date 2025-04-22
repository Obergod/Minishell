/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 16:48:21 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/22 12:31:02 by ufalzone         ###   ########.fr       */
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
	tmp->next = NULL;
	return (tmp);
}

//appelle cette fonction si lancee avec env -i
t_env *init_env(t_minishell *minishell)
{
	t_env *head;
	t_env *current;

	head = NULL;
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

static char *shell_lvl(char **result_split)
{
	if (ft_strcmp("SHLVL", result_split[0]) == 0)
	{
		return (ft_itoa(ft_atoi(result_split[1]) + 1));
	}
	return result_split[1];
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
		result_split = gc_split_first_word(envp[i], '=', minishell->gc);
		if (!result_split)
			return (NULL);

		tmp = gc_malloc(sizeof(t_env), minishell->gc);
		if (!tmp)
			return (NULL);
		tmp->key = result_split[0];
		result_split[1] = shell_lvl(result_split);
		if (!result_split[1])
			result_split[1] = "";
		tmp->value = result_split[1];
		tmp->raw = gc_strjoin_three(result_split[0], "=", result_split[1], minishell->gc);
		tmp->next = NULL;
		current->next = tmp;
		current = current->next;
	}
	check_env(head, minishell);
	return (head);
}

char **convert_t_env_to_array(t_minishell *minishell)
{
	char **result;
	t_env *current;
	int size;

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
// A voir avec Hugo ancinennement a [size] mais causait des leaks
	result[size - 1] = NULL;
	return (result);
}

int edit_in_env(char *key, char *new_value, t_minishell *minishell)
{
	t_env *current;

	if (!key || !minishell || !minishell->env)
		return (1);
	current = minishell->env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			current->value = gc_strdup(new_value, minishell->gc);
			current->raw = gc_strjoin_three(current->key, "=", new_value, minishell->gc);
			if (!current->value || !current->raw)
				return (1);
			return (0);
		}
		current = current->next;
	}
	//ca n'a pas ete trouvee
	add_to_env(&minishell->env, create_value_env(key, new_value, minishell));
	return (2);
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
