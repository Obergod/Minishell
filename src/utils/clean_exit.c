/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:55:43 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/20 15:21:01 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/main.h"

int	clean_exit(int exit_code, t_minishell *minishell)
{
	if (minishell)
	{
		if (minishell->gc)
			gc_destroy(minishell->gc);
		rl_clear_history();
	}
	exit(exit_code);
}
