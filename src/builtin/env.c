/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:51:59 by ufalzone          #+#    #+#             */
/*   Updated: 2025/05/19 19:06:48 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtin.h"
#include "../../includes/main.h"

int	ft_env(t_minishell *minishell)
{
	t_env	*env;

	env = minishell->env;
	if (!env)
	{
		ft_putstr_fd(PROMPT, 2);
		ft_putendl_fd("env: error", 2);
		return (1);
	}
	while (env)
	{
		printf("%s\n", env->raw);
		env = env->next;
	}
	return (0);
}
