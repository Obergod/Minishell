/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:15:13 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/27 15:05:20 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/main.h"

static void	ft_echo_print_all(char **args, int is_n)
{
	int	i;

	i = -1;
	while (args[++i] != NULL)
	{
		printf("%s", args[i]);
		if (args[i + 1] != NULL)
			printf(" ");
	}
	if (is_n == 0)
		printf("\n");
}

static int	ft_echo_is_n(char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (1);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (1);
		i++;
	}
	return (0);
}

int	ft_echo(char **args)
{
	int	i;

	i = 1;
	if (!args || !args[0] || ft_strcmp(args[0], "echo") != 0)
		return (1);
	if (args[0] && args[1] == NULL)
	{
		printf("\n");
		return (0);
	}
	else if (args[1] && ft_echo_is_n(args[1]) == 0)
	{
		while (ft_echo_is_n(args[i]) == 0)
			i++;
		ft_echo_print_all(args + i, 1);
		return (0);
	}
	else if (args[1] && ft_echo_is_n(args[1]) != 0)
	{
		ft_echo_print_all(args + 1, 0);
		return (0);
	}
	return (1);
}
