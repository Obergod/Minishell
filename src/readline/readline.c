/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:46:05 by ufalzone          #+#    #+#             */
/*   Updated: 2025/05/12 20:20:16 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/readline.h"

char	*ft_add_readline(const char *prompt, char **stock,
		t_minishell *minishell)
{
	(*stock) = readline(prompt);
	update_exit_status_from_signal(minishell);
	update_exit_status_from_signal(minishell);
	if (*stock == NULL)
		return (NULL);
	return (*stock);
}
