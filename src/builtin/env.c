/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:51:59 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/22 12:29:26 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtin.h"

int	ft_env(t_minishell *minishell)
{
	t_env	*env;

	env = minishell->env;
	if (!env)
	{
		printf("Erreur: ft_env.");
		return (1);
	}
	while (env)
	{
		printf("%s\n", env->raw);
		env = env->next;
	}
	return (0);
}
