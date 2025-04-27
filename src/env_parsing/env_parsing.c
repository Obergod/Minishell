/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 16:48:21 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/27 15:31:29 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/env_parsing.h"

// appelle cette fonction si lancee avec env -i
t_env	*init_env(t_minishell *minishell)
{
	t_env	*head;
	t_env	*current;

	head = NULL;
	current = create_value_env("PWD", getcwd(NULL, 0), minishell);
	add_to_env(&head, current);
	current = create_value_env("SHLVL", "1", minishell);
	add_to_env(&head, current);
	current = create_value_env("_", "/usr/bin/env", minishell);
	add_to_env(&head, current);
	return (head);
}

// il faut dans tous les cas avoir _=/usr/bin/env, SHLVL, PWD
// check[0] = PWD
// check[1] = SHLVL
// check[2] = _
void	check_env(t_env *head, t_minishell *minishell)
{
	t_env	*current;
	int		check[3];

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

static char	*shell_lvl(char **result_split, t_minishell *ms)
{
	if (ft_strcmp("SHLVL", result_split[0]) == 0)
	{
		return (gc_itoa(ft_atoi(result_split[1]) + 1, ms->gc));
	}
	return (result_split[1]);
}

static int	add_env_vars(char **envp, t_env *current, t_minishell *minishell)
{
	int		i;
	t_env	*tmp;
	char	**result_split;

	i = 0;
	while (envp[++i])
	{
		result_split = gc_split_first_word(envp[i], '=', minishell->gc);
		tmp = gc_malloc(sizeof(t_env), minishell->gc);
		if (!result_split || !tmp)
			return (1);
		tmp->key = result_split[0];
		result_split[1] = shell_lvl(result_split, minishell);
		if (!result_split[1])
			result_split[1] = "";
		tmp->value = result_split[1];
		tmp->raw = gc_strjoin_three(result_split[0], "=", result_split[1],
				minishell->gc);
		tmp->next = NULL;
		current->next = tmp;
		current = current->next;
	}
	return (0);
}

t_env	*env_parsing(char **envp, t_minishell *minishell)
{
	t_env	*current;
	t_env	*head;

	if (!envp || !envp[0])
		return (init_env(minishell));
	if (init_head_env_parsing(envp, minishell, &current, &head) == 1)
		return (NULL);
	if (add_env_vars(envp, current, minishell) == 1)
		return (NULL);
	check_env(head, minishell);
	return (head);
}
