/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 18:41:23 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/18 14:57:53 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtin.h"

int ft_exit(char **args, t_minishell *minishell)
{
	//je ne sais pas quoi faire encore faut une fonction qui exit proprement etc
	if (!args || !args[0])
		return (0);
	if (ft_strcmp(args[0], "exit") != 0)
		return (0);
	printf("exit\n");
	clean_exit(minishell);
	return (0);
}
