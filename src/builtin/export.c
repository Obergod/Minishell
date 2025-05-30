/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 17:25:38 by ufalzone          #+#    #+#             */
/*   Updated: 2025/04/27 15:13:30 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/main.h"

static int	ft_export_verif_key(char *key)
{
	int	i;

	if (!key)
		return (1);
	if (!(ft_isalpha(key[0]) || key[0] == '_'))
		return (1);
	i = 1;
	while (key[i])
	{
		if (!(ft_isalnum(key[i]) || key[i] == '_'))
			return (1);
		i++;
	}
	return (0);
}

static int	ft_export_verif_value(char *value)
{
	int	i;

	if (!value)
		return (1);
	i = 0;
	while (value[i])
	{
		if (ft_isprint(value[i]) != 1)
			return (1);
		i++;
	}
	return (0);
}

// ft_putstr_fd(PROMPT, 2);
static int	ft_export_error_message(void)
{
	ft_putstr_fd("export : ", 2);
	ft_putendl_fd("not a valid identifier", 2);
	return (1);
}

int	ft_export(char **args, t_minishell *minishell)
{
	char	**result;
	int		edit_status;
	int		i;

	if (!args || !args[0] || !args[1] || (ft_strcmp(args[0], "export") != 0))
		return (0);
	i = 0;
	while (args[++i])
	{
		if (!ft_isalpha(args[i][0]) && args[i][0] != '_')
			return (ft_export_error_message());
		if (ft_strchr(args[i], '=') == NULL)
			return (0);
		result = gc_split_first_word(args[i], '=', minishell->gc);
		if (ft_export_verif_key(result[0]) != 0
			|| ft_export_verif_value(result[1]) != 0)
			return (ft_export_error_message());
		edit_status = edit_in_env(result[0], result[1], minishell);
		if (edit_status == 1)
			return (ft_export_error_message());
	}
	return (0);
}
