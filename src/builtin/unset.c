/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 18:30:33 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/05 18:40:07 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtin.h"

static void	remove_env_var(char *key, t_minishell *minishell)
{
	t_env	*current;
	t_env	*prev;

	if (ft_strcmp(key, "_") == 0)
		return ;
	current = minishell->env;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				minishell->env = current->next;
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	ft_unset(char **args, t_minishell *minishell)
{
	int	i;

	if (!args || !minishell || !minishell->env)
		return (1);
	i = 0;
	while (args[++i])
		remove_env_var(args[i], minishell);
	return (0);
}
