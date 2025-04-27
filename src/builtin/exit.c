/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 18:41:23 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/27 18:38:48 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/main.h"

static int	ft_exit_is_digit(char *args)
{
	int	i;

	i = -1;
	if (args[0] == '+' || args[0] == '-')
		i++;
	while (args[++i])
		if (ft_isdigit(args[i]) != 1)
			return (1);
	return (0);
}

static void	print_ft_exit_not_numeric(char *args)
{
	ft_putstr_fd(PROMPT, 2);
	ft_putstr_fd("exit: ", 2);
	ft_putstr_fd(args, 2);
	ft_putendl_fd(": numeric argument required", 2);
}

int	ft_exit(char **args, t_minishell *minishell)
{
	int	status;

	if (!args || !args[0] || ft_strcmp(args[0], "exit") != 0)
		return (0);
	ft_putstr_fd("exit\n", 2);
	if (!args[1])
		clean_exit(minishell->exit_status, minishell);
	if (ft_exit_is_digit(args[1]) == 1)
	{
		print_ft_exit_not_numeric(args[1]);
		clean_exit(2, minishell);
	}
	else if (args[2])
	{
		ft_putstr_fd(PROMPT, 2);
		ft_putendl_fd("exit: too many arguments", 2);
		return (1);
	}
	else
	{
		status = ((ft_atoi(args[1])) % 256 + 256) % 256;
		clean_exit(status, minishell);
		return (status);
	}
	return (minishell->exit_status);
}
