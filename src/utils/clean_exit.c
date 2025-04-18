/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:55:43 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/18 14:57:40 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/main.h"
#include <readline/readline.h>

int clean_exit(t_minishell *minishell)
{
	gc_destroy(minishell->gc);
	rl_clear_history();
	exit(0);
}
