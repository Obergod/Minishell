/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:17:04 by ufalzone          #+#    #+#             */
/*   Updated: 2025/05/19 15:23:22 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtin.h"
#include "../../includes/main.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// cd doit etre execute dans le shell principal
// sinon le changement de dossier ne sera pas garde

static int	access_cd(char *path, t_minishell *minishell)
{
	char	*old_pwd;
	char	*pwd;

	gc_alloc(old_pwd = getcwd(NULL, 0), minishell->gc);
	if (!old_pwd)
	{
		ft_putstr_fd(PROMPT, 2);
		ft_putendl_fd("cd: too many arguments", 2);
		return (1);
	}
	if (chdir(path) == -1)
	{
		perror("chdir:");
		return (1);
	}
	edit_in_env("OLDPWD", old_pwd, minishell);
	gc_alloc(pwd = getcwd(NULL, 0), minishell->gc);
	if (!pwd)
	{
		ft_putstr_fd(PROMPT, 2);
		ft_putendl_fd("cd: too many arguments", 2);
		return (1);
	}
	edit_in_env("PWD", pwd, minishell);
	return (0);
}

int	ft_cd(char **args, t_minishell *minishell)
{
	char	*target;

	if (!args || !args[0] || ft_strcmp(args[0], "cd") != 0)
		return (1);
	if (args[2])
	{
		ft_putstr_fd(PROMPT, 2);
		ft_putendl_fd("cd: too many arguments", 2);
		return (1);
	}
	if (args[1])
		target = args[1];
	else
	{
		target = find_in_env("HOME", minishell);
		if (!target)
		{
			ft_putstr_fd(PROMPT, 2);
			ft_putendl_fd("cd: HOME not set", 2);
			return (1);
		}
	}
	return (access_cd(target, minishell));
}
